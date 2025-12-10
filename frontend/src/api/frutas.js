import axios from 'axios';

const API_URL = 'http://localhost:4000/frutas';


export const list_frutas = async (search = "") => {
  try {
    console.log(search);
    const response = await axios.get(`${API_URL}/list`, {
      params: { search }
    });
    return response.data;
  } catch (error) {
    console.error('Erro ao ir buscar lista de Area!');
    throw error;
  }
}

export const update_fruta = async (id, preco) => {
  try {
    const response = await axios.post(`${API_URL}/update/${id}`, {
      preco
    });
    console.log(preco);
    return response.data;
  } catch (error) {
    console.error('Erro ao editar produto');
    throw error;
  }
}