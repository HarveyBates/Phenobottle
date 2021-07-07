import React, { useContext, useState } from 'react';
import { UserContext } from "../usercontext.js";
import "../styles/login.css";
import Algae from "../../images/algae.png";

export function Login(){
	const {username, setUsername} = useContext(UserContext);
	const {password, setPassword} = useContext(UserContext);
	const [siUsername, setsiUsername] = useState("");
	const [siPassword, setsiPassword] = useState("");

	const updateUsername = (event) => {
		setsiUsername(event.target.value);
	};

	const updatePassword = (event) => {
		setsiPassword(event.target.value);
	}

	return (
		<div className="card">
			<div className="signin">
				<h1>Sign in</h1>
				<form className="loginform">
					<label className="loginlabel">
						<input type="text" 
							placeholder="Username" required value={siUsername} onChange={updateUsername}/>
					</label>
					<label className="loginlabel">
						<input type="password" 
							placeholder="Password" required value={siPassword} onChange={updatePassword}/>
					</label>
					<div className="loginbuttons">
						<button onClick={ async () => { 
							setUsername({siUsername})
							setPassword({siPassword})
							}
						} 
						className="submitbutton">Login</button>
						<button className="submitbutton">Sign up</button>
					</div>
				</form>
			</div>
			<div className="signup">
				<img alt="algae"
					src={Algae}
					className="algae"/>
				<h1>Sign up</h1>
				<form className="loginform">
					<label className="loginlabel">
						<input type="text" name="name" 
							placeholder="Name"/>
					</label>
					<label className="loginlabel">
						<input type="text" name="username" 
							placeholder="Username"/>
					</label>
					<label className="loginlabel">
						<input type="password" name="password" 
							placeholder="Password"/>
					</label>
					<label className="loginlabel">
						<input type="password" name="retypepassword" 
							placeholder="Retype Password"/>
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

