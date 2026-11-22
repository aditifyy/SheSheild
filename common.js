
const user = localStorage.getItem("sheshield_user") || ""

function $(id) {
    return document.getElementById(id);
}

async function api(url, data) {
    let options = {};

    if (data) {
        options = {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        };
    }

    let response = await fetch(url, options);
    return response.json();
}

function guard() {
    if (user === "") window.location.href = "/";
}

function header() {
    if ($("who")) $("who").textContent = user;
}

function logout() {
    localStorage.removeItem("sheshield_user");
    window.location.href = "/";
}

function escapeHTML(value) {
    return String(value).replace(/[&<>"']/g, "");
}

function getCurrentPosition() {
    return new Promise(function(resolve, reject) {
        navigator.geolocation.getCurrentPosition(resolve, reject);
    });
}

function googleMapsLink(lat, lon) {
    return "https://www.google.com/maps?q=" + lat + "," + lon;
}

function smsLink(phone, message) {
    return "sms:" + phone + "?body=" + encodeURIComponent(message);
}
