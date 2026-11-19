
guard();
header();

async function loadContacts() {
    let contacts = await api("/api/contacts?user=" + user);
    let list = $("contactList");

    list.innerHTML = "";

    if (contacts.length === 0) {
        list.innerHTML = "No contacts added.";
        return;
    }

    for (let i = 0; i < contacts.length; i++) {
        let c = contacts[i];

        list.innerHTML += `
            <div class="contact-row">
                <b>${c.name}</b>
                <span>${c.phone}</span>
                <button onclick="removeContact('${c.name}')">Remove</button>
            </div>
        `;
    }
}

async function addContact(event) {
    event.preventDefault();

    let name = $("contactName").value;
    let phone = $("contactPhone").value;

    if (name === "" || phone === "") return;

    let result = await api("/api/contacts/add", {
        user, name, phone
    });

    $("contactMessage").textContent = result.msg;

    if (result.ok) {
        $("contactForm").reset();
        loadContacts();
    }
}

async function removeContact(name) {
    await api("/api/contacts/remove", { user, name });
    loadContacts();
}

$("contactForm").addEventListener("submit", addContact);
loadContacts();
