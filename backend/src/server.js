require('dotenv').config();
const app = require('./app');
const connectDB = require('./config/db');

const PORT = process.env.PORT || 4000;
const URI = process.env.MONGODB_URI;

(async () => {
  try {
    await connectDB(URI);
    app.listen(PORT, () => console.log(`Server a correr em http://localhost:${PORT}`));
  } catch (err) {
    console.error('Falha ao iniciar:', err);
    process.exit(1);
  }
})();
