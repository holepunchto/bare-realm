interface EvaluateOptions {
  filename?: string
  offset?: number
}

interface Realm {
  /**
   * Evaluate `code` in the realm's isolated global scope and return its completion value. The realm
   * has its own `globalThis`, so assignments to globals made by `code` do not leak into the
   * caller's environment.
   * @param code - The JavaScript source to evaluate.
   * @param options - Options. `filename` is the script name used in stack traces (default
   * `'<anonymous>'`); `offset` shifts the reported line numbers (default `0`).
   * @returns The completion value of `code`.
   */
  evaluate(code: string, options?: EvaluateOptions): any
}

declare class Realm {
  /**
   * Get the global object of the realm that created `value`.
   * @param value - The value to look up.
   * @returns The `globalThis` of the realm that created `value`, or `null` if `value` is not an
   * object or the engine cannot tell which realm created it.
   */
  static globalOf(value: unknown): object | null

  /**
   * Check whether `value` was created by the realm whose global object is `global`. This is faster
   * than comparing the result of `Realm.globalOf()`, as it can be called directly from optimized
   * code.
   * @param value - The value to check.
   * @param global - The `globalThis` of the realm.
   * @returns `true` if `value` is an object created by the realm of `global`, and `false` otherwise.
   */
  static isFrom(value: unknown, global: object): boolean
}

declare namespace Realm {
  export type { EvaluateOptions }
}

export = Realm
