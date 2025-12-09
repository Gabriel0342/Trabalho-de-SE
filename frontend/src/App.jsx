import React from "react";
import { Routes, Route } from "react-router-dom";
import HomePage from "./view/HomePage";
import Editar from "./view/Editar";
import Layout from "./view/layout";

export default function App() {
  return (
    <Routes>
      <Route path="/" element={<Layout/>}>
        <Route path="/home" element={<HomePage />} />
        <Route path="/editar" element={<Editar />} />  
      </Route>
      
    </Routes>
  );
}
