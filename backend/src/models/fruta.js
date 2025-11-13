const { Schema, model } = require('mongoose');

const frutaSchema = new Schema(
  {
    nome: { type: String, required: [true, 'Nome obrigatório'], trim: true },
    preco: { type: Number, min: [0, 'Preço inválido'], default: 0 },
  },
  { timestamps: true }
);

frutaSchema.index({ nome: 1 }, { unique: true });

module.exports = model('Fruta', frutaSchema);
