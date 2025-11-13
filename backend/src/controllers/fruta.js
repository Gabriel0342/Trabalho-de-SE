const mongoose = require('mongoose');
const Fruta = require('../models/fruta');

exports.list = async (req, res) => {
  const frutas = await Fruta.find().sort({ createdAt: -1 }).lean();
  res.json(frutas);
};

exports.get = async (req, res) => { 
  const nome = req.params.nome || req.query.nome; 
  if (!nome) { return res.status(400).json({ erro: "Nome da fruta não especificado" }); }

  try {
    const resultado = await Fruta.findOne({ nome: nome });

    if (!resultado) { return res.status(404).json({ erro: "Fruta não encontrada" }); }

    res.json({
      fruta: resultado.nome,
      preco: resultado.preco
    });
  } catch (err) {
    console.log("Erro ao conectar", err);
    res.status(500).json({ erro: "Erro ao conectar: " + err });
  }
};

exports.create = async (req, res) => {
  const { nome, preco } = req.body;
  try {
    const novaFruta = { nome: nome, preco: preco };
    const resultado = await Fruta.insertOne(novaFruta);

    if (resultado.insertedId) {
      res.json({ mensagem: "Fruta criada com sucesso!", fruta: novaFruta, id: resultado.insertedId });
    } else {
      res.status(500).json({ mensagem: "Não foi possível criar a fruta." });
    }
  } catch (err) {
    res.status(500).json({ mensagem: "Erro ao criar fruta.", erro: err.toString() });
  } 
};

exports.update = async (req, res) => { 
  const fruta = req.body.nome || req.params.nome || req.query.nome; 
  const precoAlterado = req.body.preco || req.query.preco;

  if (!fruta || !precoAlterado) {
    return res.status(400).json({ erro: "Informe o nome da fruta e o novo preço" });
  }

  try {
    const resultado = await Fruta.updateOne(
      { nome: fruta },
      { $set: { preco: precoAlterado } }
    );

    if (resultado.matchedCount === 0) {
      return res.status(404).json({ erro: "Fruta não encontrada" });
    }

    res.json({ fruta, preco: precoAlterado, atualizado: true });
  } catch (err) {
    res.status(500).json({ erro: "Erro ao conectar: " + err });
  }
};

exports.delete = async (req, res) => {
  const { fruta } = req.body;
  try {
    const resultado = await Fruta.deleteOne({ nome: fruta });

    if (resultado.deletedCount === 1) {
      res.json({ mensagem: "Fruta apagada com sucesso!" });
    } else {
      res.status(404).json({ mensagem: "Nenhuma fruta encontrada com esse nome." });
    }
  } catch (err) {
    res.status(500).json({ mensagem: "Erro ao apagar fruta.", erro: err.toString() });
  }
};
