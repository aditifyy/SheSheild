// Current location ke coordinates
let latitude = null;
let longitude = null;


// ================= GET CURRENT LOCATION =================

function getLocation() {

    const result = document.getElementById("locationResult");

    // Check browser location support
    if (!navigator.geolocation) {

        result.innerText =
            "Location is not supported by your browser.";

        return;
    }

    // Jab location load ho rahi ho
    result.innerText =
        "Getting your location...";


    // Browser se current location lena
    navigator.geolocation.getCurrentPosition(

        function(position) {

            latitude = position.coords.latitude;
            longitude = position.coords.longitude;

            result.innerText =
                "✓ Current location detected\n" +
                "Latitude: " + latitude.toFixed(6) +
                "\nLongitude: " + longitude.toFixed(6);

        },

        function() {

            result.innerText =
                "Please allow location access.";

        }

    );
}



// ================= FIND SAFE ROUTE =================

function findSafeRoute() {

    const destination =
        document.getElementById("destination").value.trim();


    // Pehle location check
    if (latitude === null) {

        alert("Please get your current location first.");

        return;
    }


    // Destination check
    if (destination === "") {

        alert("Please enter your destination.");

        return;
    }


    // Starting location show karna
    document.getElementById("startResult").innerText =
        "Current Location";


    // Destination show karna
    document.getElementById("destinationResult").innerText =
        destination;


    // Result box show karna
    document.getElementById("routeResult").style.display =
        "block";
}



// ================= SHARE LOCATION =================

function shareLocation() {

    // Location nahi mili
    if (latitude === null) {

        alert("Please get your current location first.");

        return;
    }


    const locationText =
        "My current location: " +
        latitude.toFixed(6) +
        ", " +
        longitude.toFixed(6);


    // Agar browser sharing support karta hai
    if (navigator.share) {

        navigator.share({

            title: "SheShield Location",

            text: locationText

        });

    }

    // Agar sharing support nahi karta
    else {

        navigator.clipboard.writeText(locationText);

        alert("Location copied to clipboard!");

    }

}