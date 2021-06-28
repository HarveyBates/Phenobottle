import React from 'react';
import '../styles/devices.css';

export class DeviceHandler extends React.Component{
	render () {
		return (
			<div className="card">
				<div className="connection">
					<img alt="phenobottle logo"
						src={process.env.PUBLIC_URL + "logo.png"}
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
}




