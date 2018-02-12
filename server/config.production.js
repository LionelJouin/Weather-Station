'use strict';

module.exports = {
  restApiRoot: '/api',
  host: process.env.HOST,
  port: process.env.PORT || 1337,
  remoting: {
    context: false,
    rest: {
      handleErrors: false,
      normalizeHttpPath: false,
      xml: false,
    },
    json: {
      strict: false,
      limit: '100kb',
    },
    urlencoded: {
      extended: true,
      limit: '100kb',
    },
    cors: false,
  },
};
