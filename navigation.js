
guard();
header();

let graphData;

let positions = [
    [60, 230], [200, 110], [300, 330],
    [420, 130], [700, 130], [700, 340], [500, 260]
];


function drawMap(result) {
    let drawing = "";

    // Draw all roads according to their risk
    for (let i = 0; i < graphData.edges.length; i++) {
        let edge = graphData.edges[i];

        let start = positions[edge.from];
        let end = positions[edge.to];

        let color = "green";

        if (edge.risk >= 7) {
            color = "red";
        }
        else if (edge.risk >= 4) {
            color = "orange";
        }

        drawing += `
            <line x1="${start[0]}" y1="${start[1]}"
                  x2="${end[0]}" y2="${end[1]}"
                  stroke="${color}" stroke-width="4"/>
        `;
    }

    // Highlight safest route in purple
    if (result && result.safest) {
        let path = result.safest.path;

        for (let i = 0; i < path.length - 1; i++) {
            let a = positions[path[i]];
            let b = positions[path[i + 1]];

            drawing += `
                <line x1="${a[0]}" y1="${a[1]}"
                      x2="${b[0]}" y2="${b[1]}"
                      stroke="#7138e8" stroke-width="8"/>
            `;
        }
    }

    // Highlight shortest route in black
    if (result && result.shortest) {
        let path = result.shortest.path;

        for (let i = 0; i < path.length - 1; i++) {
            let a = positions[path[i]];
            let b = positions[path[i + 1]];

            drawing += `
                <line x1="${a[0]}" y1="${a[1]}"
                      x2="${b[0]}" y2="${b[1]}"
                      stroke="#252033" stroke-width="3"
                      stroke-dasharray="6 5"/>
            `;
        }
    }

    // Draw locations
    for (let i = 0; i < graphData.nodes.length; i++) {
        let node = graphData.nodes[i];

        let x = positions[node.id][0];
        let y = positions[node.id][1];

        drawing += `
            <circle cx="${x}" cy="${y}" r="15" fill="#7138e8"/>
            <text x="${x}" y="${y + 4}" class="node-number">${node.id}</text>
            <text x="${x}" y="${y + 32}" class="node-name">${node.name}</text>
        `;
    }

    $("routeMap").innerHTML = drawing;
}


function routeNames(route) {
    let names = "";

    for (let i = 0; i < route.path.length; i++) {
        names += graphData.nodes[route.path[i]].name;

        if (i < route.path.length - 1) names += " → ";
    }

    return names;
}

async function findRoutes() {
    let src = Number($("source").value);
    let dst = Number($("destination").value);

    if (src === dst) {
        $("routeResults").textContent = "Choose different locations.";
        return;
    }

    let result = await api("/api/route", { src, dst });

    if (!result.safest || !result.safest.found) {
        $("routeResults").textContent = "No route found.";
        return;
    }

    
$("routeResults").innerHTML = `
    <div class="route-choice safest-choice">
        <h3>Safest Route</h3>
        <p>${routeNames(result.safest)}</p>
        <span>
            Distance: ${result.safest.distance} km
            | Risk: ${result.safest.risk}
        </span>
    </div>

    <div class="route-choice shortest-choice">
        <h3>Shortest Route</h3>
        <p>${routeNames(result.shortest)}</p>
        <span>
            Distance: ${result.shortest.distance} km
            | Risk: ${result.shortest.risk}
        </span>
    </div>
`;

    drawMap(result);
}

async function loadGraph() {
    graphData = await api("/api/graph");

    let options = "";

    for (let i = 0; i < graphData.nodes.length; i++) {
        let node = graphData.nodes[i];
        options += `<option value="${node.id}">${node.name}</option>`;
    }

    $("source").innerHTML = options;
    $("destination").innerHTML = options;

    $("source").value = 0;
    $("destination").value = 5;

    drawMap();
    findRoutes();
}

$("findRoute").addEventListener("click", findRoutes);
loadGraph();
