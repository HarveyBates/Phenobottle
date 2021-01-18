const Pool = require("pg").Pool;

const pool = new Pool({
	user: "postgres",
	password: "Surfingaus1",
	host: "localhost",
	port: 5432,
	database: "phenobottle"
});

module.exports = pool;


