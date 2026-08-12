# bare-realm

Realm support for Bare.

```
npm i bare-realm
```

## Usage

```js
const Realm = require('bare-realm')

const realm = new Realm()

realm.evaluate('globalThis.foo = 42')
// 42

typeof globalThis.foo
// undefined
```

## API

See the [`bare-realm` reference](https://docs.pears.com/reference/bare/modules/bare-realm).

## License

Apache-2.0
