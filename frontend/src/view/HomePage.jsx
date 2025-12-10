import { useEffect, useState } from "react";
import { list_frutas, update_fruta } from "../api/frutas";
import Swal from "sweetalert2";
import { FiEdit3, FiSearch } from "react-icons/fi";

const Home = () => {
  const [fruits, setFruits] = useState([]);
  const [search, setSearch] = useState("");

  const fetchFruit = async (searchTerm = "") => {
    try {
      const frutas = await list_frutas(searchTerm);
      setFruits(frutas);
      console.log(frutas);
    } catch (error) {
      console.log("Erro ao encontrar frutas");
      setFruits([]);
    }
  }

  const editarFruta = async (id, nome, preco) => {
    try {
      const result = await Swal.fire({
        html: `
          <div class='flex flex-col justify-center items-center gap-5'>
            <div class='flex flex-col gap-2'>
              <h2 class='uppercase'>${nome}</h2>
              <span>Preço atual: ${preco}€</span>
            </div>
            <div class='flex items-center gap-3'>
              <label>Novo preço: </label>
              <input id='input-preco' placeholder='0,00' class='border-2 rounded-md text-center'/>
            </div>
          </div>
        `,
        showCancelButton: true,
        confirmButtonText: 'Guardar',
        confirmButtonColor: "#00C853",
        cancelButtonText: 'Cancelar',
        cancelButtonColor: "#D50000",
        preConfirm: () => {
          const input = document.getElementById('input-preco');
          const valor = input.value.trim();

          if (!valor) {
            Swal.showValidationMessage("Introduza um preço");
            return false;
          }

          return { valor };
        }
      });
      if (result.isConfirmed && result.value) {
        const novoPreco = result.value.valor;
        const data = await update_fruta(id, novoPreco);
        fetchFruit();
        console.log(data);
      }
    } catch (error) {
      Swal.fire({
        title: 'Erro ao editar preço',
        icon: 'error',
        confirmButtonText: 'OK'
      })
    }
  }

  useEffect(() => {
    fetchFruit(search);
  }, [search])

  return (
    <div className="mt-10 mx-auto max-w-5xl px-2 sm:px-4 md:px-6 lg:px-8">
      <form className="relative max-w-md mx-auto mb-6">
        <div className="absolute inset-y-0 left-0 flex items-center pl-3 pointer-events-none z-10">
          <FiSearch className="w-4 h-4 text-gray-400" />
        </div>
        <input
          type="search"
          id="search"
          className="block w-full p-3 pl-9 bg-white border border-gray-300 text-gray-900 text-sm rounded-md focus:ring-blue-500 focus:border-blue-500 placeholder:text-gray-400"
          placeholder="Procurar fruta..."
          value={search}
          onChange={(e) => setSearch(e.target.value)}
        />
      </form>
      <div className="grid grid-cols-1 gap-4">
        {fruits.map((fruta, index) => (
          <div className="bg-white-900 rounded-xl shadow-lg p-4 border grid grid-cols-3 border-black/10">
            <span className="text-xl font-semibold text-black text-start">{fruta.nome}</span>
            <span className="text-xl font-semibold text-black text-center">{Number(fruta.preco).toFixed(2).replace('.', ',')} &euro;</span>
            <button className="text-blue-600 flex items-center justify-end gap-2 w-fit ml-auto" onClick={() => {editarFruta(fruta._id, fruta.nome, fruta.preco)}}>
              <FiEdit3 />
              Editar
            </button>
          </div>
        ))}
      </div>
    </div>
  );
}

export default Home;