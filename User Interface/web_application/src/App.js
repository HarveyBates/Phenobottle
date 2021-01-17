import './App.css';
import Navbar from './navbar.jsx';

function App() {
	const sayHello = () => {
		console.log("Hello");
	};

	return (
		<div>
			<Navbar/>
			<h1> Hello </h1>
			<button onClick={sayHello}>Hello</button>
		</div>
	);
}

export default App;
