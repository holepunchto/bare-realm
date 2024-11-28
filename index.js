const binding = require('./binding')
const errors = require('./lib/errors')

module.exports = class Realm {
  constructor() {
    this._handle = binding.create()
  }

  destroy() {
    if (this._handle === null) return

    binding.destroy(this._handle)

    this._handle = null
  }

  evaluate(code, opts = {}) {
    if (this._handle === null) throw errors.REALM_DESTROYED()

    const { filename = '<anonymous>', offset = 0 } = opts

    return binding.evaluate(this._handle, code, filename, offset)
  }
}
