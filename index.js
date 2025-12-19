const binding = require('./binding')

module.exports = class Realm {
  constructor() {
    this._handle = binding.create()
  }

  evaluate(code, opts = {}) {
    const { filename = '<anonymous>', offset = 0 } = opts

    return binding.evaluate(this._handle, code, filename, offset)
  }
}
