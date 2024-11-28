module.exports = class RealmError extends Error {
  constructor(msg, code, fn = RealmError) {
    super(`${code}: ${msg}`)
    this.code = code

    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, fn)
    }
  }

  get name() {
    return 'RealmError'
  }

  static REALM_DESTROYED(msg = 'Realm has been destroyed') {
    return new RealmError(msg, 'REALM_DESTROYED', RealmError.REALM_DESTROYED)
  }
}
