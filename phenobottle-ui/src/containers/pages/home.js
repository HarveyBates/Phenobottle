import React, { useContext } from 'react';
import { UserContext } from "../usercontext.js";
import "../styles/home.css";
import logo from "../../images/logo.png";

export function Home(){
	const msg = useContext(UserContext);
	return (
		<div className="card">
			<div className="connection">
				<img alt="phenobottle logo"
					src={logo}
					className="logo"/>
				<form className="loginform">
					<label className="loginlabel">
						<input type="text" name="username" 
							placeholder="Username"/>
					</label>
					<label className="loginlabel">
						<input type="password" name="password" 
							placeholder="Password"/>
					</label>
					<div className="loginbuttons">
						<button className="submitbutton">Login</button>
						<button className="submitbutton">Sign up</button>
					</div>
				</form>
			</div>
		</div>
	);
}

