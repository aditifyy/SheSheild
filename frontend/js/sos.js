
guard();
header();

let currentLocation = null;

async function loadSOSContacts() {
    let contacts = await api("/api/contacts?user=" + user);
    let box = $("shareContacts");

    box.innerHTML = "";

    if (contacts.length === 0) {
        box.textContent = "No trusted contacts added.";
        return;
    }

    for (let i = 0; i < contacts.length; i++) {
        let c = contacts[i];
        let button = "";

        if (currentLocation) {
            let lat = currentLocation.coords.latitude;
            let lon = currentLocation.coords.longitude;
            let message = "Emergency! My location: " + googleMapsLink(lat, lon);

            button = `<a href="${smsLink(c.phone, message)}">Prepare SMS</a>`;
        }

        box.innerHTML += `
            <div class="contact-row">
                <b>${c.name}</b>
                <span>${c.phone}</span>
                ${button}
            </div>
        `;
    }
}

async function loadSOSQueue() {
    let alerts = await api("/api/sos");
    let box = $("sosQueue");

    box.innerHTML = "";

    if (alerts.length === 0) {
        box.textContent = "No SOS alerts recorded.";
        return;
    }

    for (let i = 0; i < alerts.length; i++) {
        let a = alerts[i];

        box.innerHTML += `
            <div class="activity-row">
                <b>${a.user}</b>
                <span>${a.location}</span>
                <time>${a.time}</time>
            </div>
        `;
    }
}

async function sendSOS() {
    $("sosButton").disabled = true;

    try {
        currentLocation = await getCurrentPosition();

        let lat = currentLocation.coords.latitude;
        let lon = currentLocation.coords.longitude;
        let link = googleMapsLink(lat, lon);

        $("locationDetails").hidden = false;
        $("locationDetails").innerHTML = `
            <p>Latitude: ${lat}</p>
            <p>Longitude: ${lon}</p>
            <a href="${link}" target="_blank">View on Google Maps</a>
        `;

        let result = await api("/api/sos", {
            user: user,
            location: lat + ", " + lon + " | " + link
        });

        $("sosMessage").textContent = result.ok
            ? "SOS saved successfully."
            : "Could not save SOS.";

        $("gpsStatus").textContent = "Location fetched.";

        loadSOSContacts();
        loadSOSQueue();

    } catch (error) {
        $("gpsStatus").textContent = "Could not fetch location.";
        $("sosMessage").textContent = error.message;
    }

    $("sosButton").disabled = false;
}

$("sosButton").addEventListener("click", sendSOS);

loadSOSContacts();
loadSOSQueue();
