#include <assert.h>
#include <bare.h>
#include <js.h>
#include <stdint.h>
#include <stdlib.h>

typedef js_context_t bare_realm_t;

static void
bare_realm__on_finalize(js_env_t *env, void *data, void *finalize_hint) {
  bare_realm_t *realm = (bare_realm_t *) data;

  int err = js_destroy_context(env, realm);
  assert(err == 0);
}

static js_value_t *
bare_realm_create(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_realm_t *realm;
  err = js_create_context(env, &realm);
  if (err < 0) return NULL;

  js_value_t *handle;
  err = js_create_external(env, (void *) realm, bare_realm__on_finalize, NULL, &handle);
  assert(err == 0);

  return handle;
}

static js_value_t *
bare_realm_evaluate(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  bare_realm_t *realm;
  err = js_get_value_external(env, argv[0], (void **) &realm);
  assert(err == 0);

  js_value_t *source = argv[1];

  size_t len;
  err = js_get_value_string_utf8(env, argv[2], NULL, 0, &len);
  assert(err == 0);

  utf8_t *file = malloc(len + 1 /* NULL */);
  err = js_get_value_string_utf8(env, argv[2], file, len + 1, NULL);
  assert(err == 0);

  int32_t offset;
  err = js_get_value_int32(env, argv[3], &offset);
  assert(err == 0);

  err = js_enter_context(env, realm);
  assert(err == 0);

  js_value_t *result;
  err = js_run_script(env, (char *) file, len, offset, source, &result);
  (void) err;

  free(file);

  err = js_exit_context(env, realm);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_realm_global_of(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  bool is_object;
  err = js_is_object(env, argv[0], &is_object);
  assert(err == 0);

  js_value_t *result;

  if (is_object) {
    err = js_get_object_global(env, argv[0], &result);
    assert(err == 0);
  } else {
    err = js_get_null(env, &result);
    assert(err == 0);
  }

  return result;
}

static bool
bare_realm__is_from(js_env_t *env, js_value_t *value, js_value_t *global) {
  int err;

  bool is_object;
  err = js_is_object(env, value, &is_object);
  assert(err == 0);

  if (!is_object) return false;

  js_value_t *result;
  err = js_get_object_global(env, value, &result);
  assert(err == 0);

  bool equals;
  err = js_strict_equals(env, result, global, &equals);
  assert(err == 0);

  return equals;
}

static js_value_t *
bare_realm_is_from(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  js_value_t *result;
  err = js_get_boolean(env, bare_realm__is_from(env, argv[0], argv[1]), &result);
  assert(err == 0);

  return result;
}

static bool
bare_realm_is_from_typed(js_value_t *receiver, js_value_t *value, js_value_t *global, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  bool result = bare_realm__is_from(env, value, global);

  err = js_close_handle_scope(env, scope);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_realm_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, untyped, signature, typed) \
  { \
    js_value_t *val; \
    if (signature) { \
      err = js_create_typed_function(env, name, -1, untyped, signature, typed, NULL, &val); \
      assert(err == 0); \
    } else { \
      err = js_create_function(env, name, -1, untyped, NULL, &val); \
      assert(err == 0); \
    } \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("create", bare_realm_create, NULL, NULL)
  V("evaluate", bare_realm_evaluate, NULL, NULL)
  V("globalOf", bare_realm_global_of, NULL, NULL)

  V(
    "isFrom",
    bare_realm_is_from,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_boolean,
      .args_len = 3,
      .args = (int[]){
        js_object,
        js_object,
        js_object,
      },
    }),
    bare_realm_is_from_typed
  )
#undef V

  return exports;
}

BARE_MODULE(bare_realm, bare_realm_exports)
