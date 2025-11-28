import React from "react";
import NavBar from "../components/NavBar";

export default function Editar() {
  return (
    <div className="min-h-screen bg-white text-black">
      <NavBar />

      <main className="mt-10 mx-auto max-w-screen-2xl px-4">
        <div className="bg-white- rounded-xl shadow-lg p-18 border border-black/50 max-w-xs mx-auto">
          <h1 className="text-3xl font-semibold text-black text-center">
            Nome Fruta
          </h1>
          <h4 className="text-xl font-semibold text-black text-center mt-10">
            Peso: 7Kg
          </h4>
          <h4 className="text-xl font-semibold text-black text-center mt-2">
            Nome: Morango
          </h4>
        </div>
        <div className="mt-12 grid grid-cols-1 md:grid-cols-3 gap-20  mx-auto bg-gray-100 p-6 rounded-xl">
          <div className="p-4 bg-gray rounded-xl text-center ">
            <h3 className="font-semibold text-lg ">Editar Nome</h3>
          </div>

          <div className="p-4 bg-gray rounded-xl text-center">
            <h3 className="font-semibold text-lg">Editar Preço</h3>
           
          </div>

          <div className="p-4 bg-gray rounded-xl text-center">
            <h3 className="font-semibold text-lg">Ocultar</h3>
           
          </div>
        </div>
      </main>
    </div>
  );
}
