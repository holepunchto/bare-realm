const test = require('brittle')
const Realm = require('.')

test('basic', (t) => {
  const realm = new Realm()

  const globalThat = realm.evaluate('globalThis')

  t.unlike(globalThat, globalThis)
})
