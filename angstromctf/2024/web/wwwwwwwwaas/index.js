const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');
const express = require('express');
const crypto = require('crypto');
const fs = require('fs');

let flags = ["flag{example_flag}", "aplet123ctf{the_hallmark_of_a_great_misc_challenge}"];
fs.readFile('flag.txt', 'utf8', function(err, data) {  
	if (err) throw err;
	flags.push(data);
});

const secretvalue = process.env.SECRET_KEY || crypto.randomBytes(32).toString('hex')

const app = express();

app.use(express.static('static'));
app.use(cookieParser());
app.use(bodyParser.urlencoded({extended:false}));

app.use((req, res, next) => {
	res.set('X-Frame-Options', 'deny');
	res.set('X-Content-Type-Options', 'nosniff');
	res.set('Cache-Control', 'no-store');
	next()
})

app.get('/', (req, res) => {
	res.sendFile('static/index.html',{root:__dirname});
})

app.get('/search', (req, res) => {
	if (req.cookies['admin_cookie'] !== secretvalue) {
		res.status(403).send("Unauthorized");
		return;
	}
	try {
		let query = req.query.q;
		for (let flag of flags) {
			if (flag.indexOf(query) !== -1) {
				res.status(200).send("Found");
				return;
			}
		}
		res.status(404).send("Not Found");
	} catch (e) {
		console.log(e);
		res.sendStatus(500);
	}
})

app.listen(21111,()=>console.log('running'));
