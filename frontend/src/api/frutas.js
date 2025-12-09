import axios from 'axios';

const API_URL = 'http://localhost:4000/frutas';


export const list_frutas = async () => {
  try{
    const response = await axios.get(`${API_URL}/list`);
    return response.data;
  }catch (error){
    console.error('Erro ao ir buscar lista de Area!');
    throw error;
  }
}