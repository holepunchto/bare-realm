declare class Realm {
  evaluate(code: string, opts?: { filename: string; offset: number }): any

  destroy(): void
}

export = Realm
