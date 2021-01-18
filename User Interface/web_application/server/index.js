const express = require('express');
const app = express();
const cors = require('cors');
const pool = require('./db');

app.use(cors());
app.use(express.json());

// Set a parameter value
app.post("/parameters", async(req, res) => {
	try{
		const {description} = req.body;
		const newParameter = await pool.query(
			"INSERT INTO parameters (description) VALUES ($1) RETURNING *",
			[description]);
		res.json(newParameter.rows[0]);
	} 
	catch (err){
		console.error(err.message);
	}
});

// Get all Parameters
app.get("/parameters", async(req, res) => {
	try{
		const allParameters = await pool.query(
			"SELECT * from parameters");
		res.json(allParameters.rows);
	} 
	catch(err){
		console.error(err.message);
	}
});

// Get a parameter
app.get("/parameters/:id", async(req, res) => {
	try {
		const {id} = req.params;
		const getParameter = await pool.query(
			"SELECT * from parameters WHERE phenobottle = $1",
			[id]);
		res.json(getParameter.rows);
	}
	catch(err) {
		console.error(err.message);
	}
});

// Update a parameter
app.put("parameters/:id", async(req, res) => {
	try{
		const {id} = req.params;
		const {description} = req.body;
		const updateParameter = await pool.query(
			"UPDATE parameters SET description = $1 WHERE phenobottle = $2", [description, id]);
		res.json(updateParameter.rows);
	}
	catch(err){
		console.error(err.message);
	}
});

// Delete a parameter
app.delete("/parameters/:id", async(req, res) => {
	try{
		const {id} = req.params;
		const delParameter = await pool.query(
			"DELETE FROM parameters where phenobottle = $1",
			[id]);
		res.json("Item Deleted");
	}
	catch(err){
		console.log(err.message);
	}
});

app.listen(5000, () => {
	console.log("server has started");
});

