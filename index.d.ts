interface EvaluateOptions {
  filename?: string
  offset?: number
}

interface Realm {
  evaluate(code: string, options?: EvaluateOptions): any
}

declare class Realm {}

declare namespace Realm {
  export type { EvaluateOptions }
}

export = Realm
