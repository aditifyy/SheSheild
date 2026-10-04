import "./App.css";

function App() {
  return (
    <div>
      <nav className="navbar">
        <h2 className="logo">SheShield</h2>

        <div className="nav-links">
          <a href="#">Home</a>
          <a href="#">Navigation</a>
          <a href="#">SOS</a>
          <a href="#">Profile</a>
        </div>
      </nav>

      <div className="container">
        <h1>Welcome to SheShield</h1>

        <p className="subtitle">
          Your safety, our priority.
        </p>

        <div className="card-container">

          <div className="card">
            <h2>🗺️ Safe Navigation</h2>
            <p>Find a safer route to your destination.</p>
            <button className="btn">Explore</button>
          </div>

          <div className="card">
            <h2>🚨 SOS Alert</h2>
            <p>Get emergency assistance when needed.</p>
            <button className="sos-btn">SOS</button>
          </div>

          <div className="card">
            <h2>📍 Live Location</h2>
            <p>Share your location with trusted contacts.</p>
            <button className="btn">View</button>
          </div>

          <div className="card">
            <h2>👥 Emergency Contacts</h2>
            <p>Manage your emergency contacts.</p>
            <button className="btn">View</button>
          </div>

        </div>
      </div>
    </div>
  );
}

export default App;
