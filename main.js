const fruta = "Banana";
const peso = 2;

const { MongoClient } = require('mongodb');
const { sourceMapsEnabled } = require('process');
const uri = 'mongodb+srv://123balanca123:3tsi0TO76yzwR9EM@balanca.mxt1psj.mongodb.net/';
const cliente = new MongoClient(uri);

exports.lerPrecoFruta = async (req, res) => { //Read
  const fruta = req.params.nome || req.query.nome; 
  if (!fruta) {
    return res.status(400).json({ erro: "Nome da fruta não especificado" });
  }

  try {
    await cliente.connect();
    const baseDados = cliente.db("Balanca");
    const colecao = baseDados.collection("Fruta");

    const resultado = await colecao.findOne({ nome: fruta });

    if (!resultado) {
      return res.status(404).json({ erro: "Fruta não encontrada" });
    }

    res.json({
      fruta: resultado.nome,
      preco: resultado.preço
    });
  } catch (err) {
    console.log("Erro ao conectar", err);
    res.status(500).json({ erro: "Erro ao conectar: " + err });
  } finally {
    await cliente.close();
  }
};

exports.alterarPrecoFruta = async (req, res) => { // Update
  const fruta = req.body.nome || req.params.nome || req.query.nome; // nome da fruta
  const precoAlterado = req.body.preco || req.query.preco; // novo preço

  if (!fruta || !precoAlterado) {
    return res.status(400).json({ erro: "Informe o nome da fruta e o novo preço" });
  }

  try {
    await cliente.connect();
    const baseDados = cliente.db("Balanca");
    const colecao = baseDados.collection("Fruta");

    const resultado = await colecao.updateOne(
      { nome: fruta },
      { $set: { preco: precoAlterado } }
    );

    if (resultado.matchedCount === 0) {
      return res.status(404).json({ erro: "Fruta não encontrada" });
    }

    res.json({ fruta, preco: precoAlterado, atualizado: true });
  } catch (err) {
    console.log("Erro ao conectar", err);
    res.status(500).json({ erro: "Erro ao conectar: " + err });
  } finally {
    await cliente.close();
  }
};

exports.apagar = async (req, res) => { //Delete
  const { fruta } = req.body; // ou req.params, dependendo de como envia o dado
  try {
    await cliente.connect();

    const baseDados = cliente.db("Balanca");
    const colecao = baseDados.collection("Fruta");

    const resultado = await colecao.deleteOne({ nome: fruta });

    if (resultado.deletedCount === 1) {
      res.json({ mensagem: "Fruta apagada com sucesso!" });
    } else {
      res.status(404).json({ mensagem: "Nenhuma fruta encontrada com esse nome." });
    }
  } catch (err) {
    res.status(500).json({ mensagem: "Erro ao apagar fruta.", erro: err.toString() });
  } finally {
    await cliente.close();
  }
};

exports.criar = async (req, res) => { //Create
  const { nome, preco } = req.body; // espera receber nome e preco no corpo da requisição
  try {
    await cliente.connect();

    const baseDados = cliente.db("Balanca");
    const colecao = baseDados.collection("Fruta");

    const novaFruta = { nome: nome, preco: preco };
    const resultado = await colecao.insertOne(novaFruta);

    if (resultado.insertedId) {
      res.json({ mensagem: "Fruta criada com sucesso!", fruta: novaFruta, id: resultado.insertedId });
    } else {
      res.status(500).json({ mensagem: "Não foi possível criar a fruta." });
    }
  } catch (err) {
    res.status(500).json({ mensagem: "Erro ao criar fruta.", erro: err.toString() });
  } finally {
    await cliente.close();
  }
};



