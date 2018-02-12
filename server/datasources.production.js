'use strict';

module.exports = {
  db: {
    name: 'db',
    connector: 'cloudant',
    url: process.env.DB_URL,
    database: 'weatherstation',
  },
};
