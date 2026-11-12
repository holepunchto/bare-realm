const test = require('brittle')
const Realm = require('.')

test('basic', (t) => {
  const realm = new Realm()

  const globalThat = realm.evaluate('globalThis')

  t.unlike(globalThat, globalThis)
})

test('globalOf', (t) => {
  const realm = new Realm()
  const global = realm.evaluate('globalThis')

  t.is(Realm.globalOf(realm.evaluate('({})')), global)
  t.is(Realm.globalOf(realm.evaluate('(function () {})')), global)
  t.is(Realm.globalOf({}), globalThis)
  t.is(Realm.globalOf(42), null)
  t.is(Realm.globalOf(null), null)
})

test('isFrom', (t) => {
  const realm = new Realm()
  const global = realm.evaluate('globalThis')

  t.ok(Realm.isFrom(realm.evaluate('({})'), global))
  t.ok(Realm.isFrom(global, global))
  t.absent(Realm.isFrom({}, global))
  t.ok(Realm.isFrom({}, globalThis))
  t.absent(Realm.isFrom(42, global))
})

test('isFrom from optimized code', (t) => {
  const realm = new Realm()
  const global = realm.evaluate('globalThis')
  const values = [realm.evaluate('({})'), {}, 42]

  let matches = 0

  for (let i = 0; i < 1e6; i++) {
    if (Realm.isFrom(values[i % 3], global)) matches++
  }

  t.is(matches, Math.ceil(1e6 / 3))
})
