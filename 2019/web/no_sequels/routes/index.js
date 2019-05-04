var express = require('express');
var router = express.Router();
var path = require('path');
var jwtm = require('express-jwt');
var jwt = require('jsonwebtoken')

var secret = 'd0b26fad0443d42d08b334d56a005ebd'

router.get('/', function(req, res, next) {
    res.cookie('token', jwt.sign({authenticated: false}, secret));
  res.redirect("/login");
});

router.get('/login', function (req, res) {
    res.cookie('token', jwt.sign({authenticated: false}, secret));
    res.sendFile(__dirname + '/home.html');
});

verifyJwt = jwtm({secret, getToken: function (req) {
    if (req.cookies && req.cookies.token) return req.cookies.token
    return null
}})

router.post('/login', verifyJwt, function (req, res) {
    // monk instance
    var db = req.db;

    var user = req.body.username;
    var pass = req.body.password;

    if (!user || !pass){
        res.send("One or more fields were not provided.");
    }
    var query = {
        username: user,
        password: pass
    }

    db.collection('users').findOne(query, function (err, user) {
        if (!user){
            res.send("Wrong username or password");
            return
        }

        res.cookie('token', jwt.sign({name: user.username, authenticated: true}, secret));
        res.redirect("/site");
    });
});

router.get('/site', verifyJwt, function (req, res) {
    // req.user is assigned from verifyJwt
    if (!req.user.authenticated) {
        res.send("bad");
    }
    console.log(req.user)
    res.render("access", {username: ' \'' + req.user.name + '\' '})
});

router.post('/site', verifyJwt, function (req, res) {
    // req.user is assigned from verifyJwt
    if (!req.user.authenticated || !req.body.pass2) {
        res.send("bad");
    }

    var query = {
        username: req.user.name,
    }

    var db = req.db;
    db.collection('users').findOne(query, function (err, user) {
        console.log(user);
        if (!user){
            res.render('access', {username:' \''+req.user.name+'\' ', message:"Only user 'admin' can log in with this form!"});
        }
        var pass = user.password;
        var message = "";
        if (pass === req.body.pass2){
            res.render('final');
        } else {
            res.render('access', {username:' \''+req.user.name+'\' ', message:"Wrong LOL!"});
        }

    });

});

module.exports = router;
