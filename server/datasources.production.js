'use strict';

module.exports = {
  db: {
    name: 'db',
    connector: 'cloudant',
    url: process.env.DB_URL,
    database: 'weatherstation',
  },
  AzureDB: {
    name: "AzureDB",
    connector: "mssql",
    host: process.env.DB_AZURE_HOST,
    port: process.env.DB_AZURE_PORT,
    database: process.env.DB_AZURE_DATABASE,
    username: process.env.DB_AZURE_USERNAME,
    password: process.env.DB_AZURE_PASSWORD,
    options: {
      encrypt: true,
      database: process.env.DB_AZURE_DATABASE
    }
  }
};
