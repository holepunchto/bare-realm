const test = require('brittle')
const Realm = require('.')

test('basic', (t) => {
  const realm = new Realm()
  t.teardown(() => realm.destroy())

  const globalThat = realm.evaluate('globalThis')

  t.unlike(globalThat, globalThis)
})
