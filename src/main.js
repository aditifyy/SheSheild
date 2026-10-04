import "./style.css";

document.querySelector("#app").innerHTML = `
  <nav class="navbar">
    <h2 class="logo">SheShield</h2>

    <div class="nav-links">
      <a href="#">Home</a>
      <a href="#">Navigation</a>
      <a href="#">SOS</a>
      <a href="#">Profile</a>
    </div>
  </nav>

  <main class="container">

    <h1>Welcome to SheShield</h1>

    <p class="subtitle">
      Your safety, our priority.
    </p>

    <div class="card-container">

      <div class="card">
        <h2>🗺️ Safe Navigation</h2>
        <p>Find a safer route to your destination.</p>
        <button class="btn">Explore</button>
      </div>

      <div class="card">
        <h2>🚨 SOS Alert</h2>
        <p>Get emergency assistance when needed.</p>
        <button class="sos-btn">SOS</button>
      </div>

      <div class="card">
        <h2>📍 Live Location</h2>
        <p>Share your location with trusted contacts.</p>
        <button class="btn">View</button>
      </div>

      <div class="card">
        <h2>👥 Emergency Contacts</h2>
        <p>Manage your emergency contacts.</p>
        <button class="btn">View</button>
      </div>

    </div>

  </main>
`;