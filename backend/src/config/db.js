const mongoose = require('mongoose');

async function connectDB(uri) {
  mongoose.set('strictQuery', true);
  await mongoose.connect(uri);
  const { host, name } = mongoose.connection;
  console.log(`MongoDB ligado: ${host}/${name}`);
}

module.exports = connectDB;
