#include <assert.h>
#include <bare.h>
#include <js.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  js_context_t *context;
} bare_realm_t;

static void
on_realm_finalize (js_env_t *env, void *data, void *finalize_hint) {
  bare_realm_t *realm = data;

  js_destroy_context(env, realm->context);
}

static js_value_t *
bare_realm_create (js_env_t *env, js_callback_info_t *info) {
  int err;

  js_value_t *handle;

  bare_realm_t *realm;
  err = js_create_arraybuffer(env, sizeof(bare_realm_t), (void **) &realm, &handle);
  assert(err == 0);

  err = js_create_context(env, &realm->context);
  if (err < 0) return NULL;

  err = js_add_finalizer(env, handle, (void *) realm, on_realm_finalize, NULL, NULL);
  assert(err == 0);

  return handle;
}

static js_value_t *
bare_realm_destroy (js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  bare_realm_t *realm;
  err = js_get_arraybuffer_info(env, argv[0], (void **) &realm, NULL);
  assert(err == 0);

  js_destroy_context(env, realm->context);

  return NULL;
}

static js_value_t *
bare_realm_evaluate (js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  bare_realm_t *realm;
  err = js_get_arraybuffer_info(env, argv[0], (void **) &realm, NULL);
  assert(err == 0);

  js_value_t *source = argv[1];

  size_t len;
  err = js_get_value_string_utf8(env, argv[2], NULL, 0, &len);
  assert(err == 0);

  len += 1 /* NULL */;

  utf8_t *file = malloc(len);
  err = js_get_value_string_utf8(env, argv[2], file, len, NULL);
  assert(err == 0);

  int32_t offset;
  err = js_get_value_int32(env, argv[3], &offset);
  assert(err == 0);

  err = js_enter_context(env, realm->context);
  assert(err == 0);

  js_value_t *result;
  err = js_run_script(env, (char *) file, len, offset, source, &result);
  (void) err;

  free(file);

  err = js_exit_context(env, realm->context);
  assert(err == 0);

  return result;
}

static js_value_t *
bare_realm_exports (js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("create", bare_realm_create)
  V("destroy", bare_realm_destroy)
  V("evaluate", bare_realm_evaluate)
#undef V

  return exports;
}

BARE_MODULE(bare_realm, bare_realm_exports)
