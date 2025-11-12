const express = require('express');
const morgan = require('morgan');
const cors = require('cors');

const frutasRoutes = require('./routes/frutas');
const app = express();

app.use(cors());
app.use(express.json()); 
app.use(morgan('dev'));

app.use('/frutas', frutasRoutes);

module.exports = app;
