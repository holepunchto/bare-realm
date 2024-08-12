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

#### `const realm = new Realm()`

#### `realm.destroy()`

#### `const result = realm.evaluate(code[, options])`

Options include:

```js
{
  filename: '<anonymous>',
  offset: 0
}
````

## License

Apache-2.0
