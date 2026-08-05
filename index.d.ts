interface EvaluateOptions {
  filename?: string
  offset?: number
}

interface Realm {
  /**
   * Evaluate `code` in the realm's isolated global scope and return its completion value. The realm has its own `globalThis`, so assignments to globals made by `code` do not leak into the caller's environment.
   * @param code - The JavaScript source to evaluate.
   * @param options - Options. `filename` is the script name used in stack traces (default `'<anonymous>'`); `offset` shifts the reported line numbers (default `0`).
   * @returns The completion value of `code`.
   */
  evaluate(code: string, options?: EvaluateOptions): any
}

declare class Realm {}

declare namespace Realm {
  export type { EvaluateOptions }
}

export = Realm
