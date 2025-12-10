const express = require('express');
const controller = require('../controllers/fruta');
const router = express.Router();

router.get('/list', controller.list);
router.get('/get/:nome', controller.get);
router.post('/create', controller.create);
router.post('/update/:id', controller.update);
router.delete('/delete/:nome', controller.delete);

module.exports = router; 