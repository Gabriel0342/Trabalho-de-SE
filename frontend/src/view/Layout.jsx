import { Outlet } from "react-router-dom";
import NavBar from "../components/NavBar";

const Layout = () =>  {
    return(
        <div>
            <NavBar/>
            <div className="min-h-screen bg-blue-20 text-white">
                <Outlet/>    
            </div>
        </div>
    )
}

export default Layout;