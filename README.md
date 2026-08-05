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

<!-- bare-refgen:api start -->

## API

### Realm

#### `evaluate(code: string, options?: EvaluateOptions): any`

Evaluate `code` in the realm's isolated global scope and return its completion value. The realm has its own `globalThis`, so assignments to globals made by `code` do not leak into the caller's environment.

**Parameters**

| Parameter  | Type              | Default | Description                                                                                                                                     |
| ---------- | ----------------- | ------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| `code`     | `string`          | —       | The JavaScript source to evaluate.                                                                                                              |
| `options?` | `EvaluateOptions` | —       | Options. `filename` is the script name used in stack traces (default `'<anonymous>'`); `offset` shifts the reported line numbers (default `0`). |

**Returns** `any` — The completion value of `code`.

### Types

#### `EvaluateOptions`

```ts
interface EvaluateOptions {
  filename?: string
  offset?: number
}
```

<!-- bare-refgen:api end -->

## License

Apache-2.0
