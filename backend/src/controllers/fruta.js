const mongoose = require('mongoose');
const Fruta = require('../models/fruta');

exports.list = async (req, res) => {
  const frutas = await Fruta.find().sort({ createdAt: -1 }).lean();
  res.json(frutas);
};