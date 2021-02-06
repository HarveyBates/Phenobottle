const express = require('express');
const app = express();
const cors = require('cors');
const pool = require('./db');

app.use(cors());
app.use(express.json());

// Set a parameter value
app.post("/parameters", async(req, res) => {
	try{
		const {identity, condition, opticaldensity, 
				quantumyield, ph, temperature, time} = req.body;
		const newParameter = await pool.query(
			"INSERT INTO parameters (identity, condition, opticaldensity, quantumyield, ph, temperature, time) VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING *",
			[identity, condition, opticaldensity, 
				quantumyield, ph, temperature, time]);
		res.json(newParameter.rows[0]);
	} 
	catch (err){
		console.error(err.message);
	}
});

// Get all Parameters
app.get("/parameters", async(req, res) => {
	console.log(req.query.identity);
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
app.get("/parameters/:identity", async(req, res) => {
	try {
		const {identity} = req.params;
		const getParameter = await pool.query(
			"SELECT * from parameters WHERE identity = $1",
			[identity]);
		res.json(getParameter.rows);
	}
	catch(err) {
		console.error(err.message);
	}
});

// Update a parameter
app.put("parameters/:identity", async(req, res) => {
	try{
		const {identity} = req.params;
		const {condition, opticaldensity,
				quantumyield, ph, temperature, time} = req.body;
		const updateParameter = await pool.query(
			"UPDATE parameters SET (identity, condition, opticaldensity, quantumyield, ph, temperature, time) = ($1, $2, $3, $4, $5, $6) WHERE identity = $7", 
			[condition, opticaldensity,
				quantumyield, ph, temperature, time, identity]);
		res.json(updateParameter.rows);
	}
	catch(err){
		console.error(err.message);
	}
});

// Delete a parameter
app.delete("/parameters/:identity", async(req, res) => {
	try{
		const {identity} = req.params;
		const delParameter = await pool.query(
			"DELETE FROM parameters WHERE identity = $1",
			[identity]);
		res.json("Item Deleted");
	}
	catch(err){
		console.log(err.message);
	}
});

app.listen(5000, () => {
	console.log("server has started");
});

