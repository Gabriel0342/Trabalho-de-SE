import React from "react";
import { Routes, Route } from "react-router-dom";
import HomePage from "./view/HomePage";
import Editar from "./view/Editar";

export default function App() {
  return (
    <Routes>
      <Route path="/" element={<HomePage />} />
      <Route path="/editar" element={<Editar />} />
    </Routes>
  );
}
