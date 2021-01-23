

let socket = new WebSocket("wss://localhost:15001");

socket.onopen = function(e) {
	alert("[open] Connection established");
	alert("Sending to server");
	socket.send("Testing");
};

socket.onclose = function(event){
	if(event.wasClean){
		alert(`[close] Connection was close cleanly, code=${event.code} reason=${event.reason}`);
	} else{
		alert('[close] Connection died');
	}
};

socket.onerror = function(error){
	alert(`[error] ${error.message}`);
}

const send = () =>{
	socket.send("Button pressed");
};

const Setup = () => {
		return (
			<>
			<div className="setup">
			<h1>SETUP</h1>
			<div className="graph-card">
				<div className="update-pb"> 
					<button onClick={send} className="btn-refresh">Update
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



