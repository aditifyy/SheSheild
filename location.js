
guard();
header();

let latestPosition = null;
let watchId = null;
let savedContacts = [];

function showPosition(position) {
    latestPosition = position;

    let lat = position.coords.latitude;
    let lon = position.coords.longitude;

    $("latitude").textContent = lat;
    $("longitude").textContent = lon;
    $("accuracy").textContent = position.coords.accuracy + " metres";
    $("lastUpdated").textContent = new Date().toLocaleTimeString();

    $("locationStatus").textContent = "Location updated";

    $("mapPreview").innerHTML =
        `<a href="${googleMapsLink(lat, lon)}" target="_blank">Open Google Maps</a>`;

    showShareOptions();
}

function locationError(error) {
    $("locationStatus").textContent = "Location unavailable";
    $("shareMessage").textContent = error.message;
}

async function fetchLocation() {
    $("locationStatus").textContent = "Fetching location...";

    try {
        let position = await getCurrentPosition();
        showPosition(position);
    } catch (error) {
        locationError(error);
    }
}

function startLiveUpdates() {
    if (watchId !== null) return;

    watchId = navigator.geolocation.watchPosition(showPosition, locationError);

    $("watchButton").disabled = true;
    $("stopButton").disabled = false;
}

function stopLiveUpdates() {
    if (watchId !== null) {
        navigator.geolocation.clearWatch(watchId);
        watchId = null;
    }

    $("watchButton").disabled = false;
    $("stopButton").disabled = true;
}

async function loadContacts() {
    savedContacts = await api("/api/contacts?user=" + user);
    showShareOptions();
}

function showShareOptions() {
    let box = $("locationContacts");

    if (!latestPosition || savedContacts.length === 0) {
        box.innerHTML = "Fetch location and add trusted contacts first.";
        return;
    }

    let lat = latestPosition.coords.latitude;
    let lon = latestPosition.coords.longitude;
    let message = "My location: " + googleMapsLink(lat, lon);

    box.innerHTML = "";

    for (let i = 0; i < savedContacts.length; i++) {
        let c = savedContacts[i];

        box.innerHTML += `
            <div class="contact-row">
                <b>${c.name}</b>
                <span>${c.phone}</span>
                <a href="${smsLink(c.phone, message)}">Prepare SMS</a>
            </div>
        `;
    }
}

$("fetchButton").addEventListener("click", fetchLocation);
$("watchButton").addEventListener("click", startLiveUpdates);
$("stopButton").addEventListener("click", stopLiveUpdates);

loadContacts();
