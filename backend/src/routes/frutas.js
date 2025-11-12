const express = require('express');
const controller = require('../controllers/fruta');
const router = express.Router();

router.get('/list', controller.list);

module.exports = router;