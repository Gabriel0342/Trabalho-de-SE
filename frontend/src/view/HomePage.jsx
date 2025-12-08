import NavBar from "../components/NavBar";

export default function App() {
  return (
    <div className="min-h-screen bg-blue-20 text-white">
      <NavBar />

      <main className="mt-10 mx-auto max-w-screen-2xl px-2 sm:px-4 md:px-6 lg:px-8">
        <h2 className="text-2xl font-semibold text-black mb-10">Listagem</h2>
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          <a href="/editar">
            <div className="bg-white-900 rounded-xl shadow-lg p-4 border border-black/10">
              <h3 className="text-xl font-semibold text-black text-center">
                Nome Fruta
              </h3>
            </div>
          </a>
          <div className="bg-white-900 rounded-xl shadow-lg p-4 border border-black/10">
              <h3 className="text-xl font-semibold text-black text-center">
                Pesquisa da fruta?
              </h3>
            </div>
        </div>
        
      </main>
    </div>
  );
}
