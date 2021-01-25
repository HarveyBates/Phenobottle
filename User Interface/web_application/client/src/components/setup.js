

let socket = new WebSocket("ws://localhost:15001");

socket.onopen = function(evt) {
	console.log(evt);
	alert("Connection established");
};

socket.onmessage = function(evt){
	alert(evt.data);
};

socket.onclose = function(evt){
	alert(evt.data);
};

socket.onerror = function(evt){
	alert(evt.data);
};

const send_message = () => {
	socket.send("Hello!");
}

const Setup = () => {
		return (
			<>
			<div className="setup">
			<h1>SETUP</h1>
			<div className="graph-card">
				<div className="update-pb"> 
					<button onClick={send_message} className="btn-refresh">Update
						<i className="fa fa-refresh"></i>
					</button>
				</div>
				<p>This is a page to setup Phenobottles</p>
				<div className="graph-card">
					<h2> Phenobottle Num. 1</h2>
					<p> v0.1 Online. </p>
				</div>
			</div>
			</div>
			</>
		)
}

export default Setup;



