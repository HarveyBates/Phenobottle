const express = require('express');
const app = express();
const process = require('process');

//Middleware - Runs when route is selected (could be used for checking auth)
//app.use("/", () => {
//	console.log("Hello World!");
//});


// Route
app.get('/', (req, res) => {
	res.send("We are on home");
});

app.listen(3000);




