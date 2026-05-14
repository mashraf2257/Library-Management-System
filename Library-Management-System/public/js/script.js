// ================= ID GENERATOR =================
function generateMemberID() {
    const letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    const l1 = letters[Math.floor(Math.random() * 26)];
    const l2 = letters[Math.floor(Math.random() * 26)];
    const nums = String(Math.floor(1000 + Math.random() * 9000));
    return l1 + l2 + nums;
}

// ================= LOGIN & LOGOUT =================
const loginPage = document.getElementById("login-page");
const dashboard  = document.getElementById("dashboard");
const loginForm  = document.getElementById("login-form");
const loginError = document.getElementById("login-error");

loginForm.addEventListener("submit", function (e) {
    e.preventDefault();
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    // Login via server-side API endpoint
    fetch("/api/login", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ username, password })
    })
    .then(res => res.json())
    .then(data => {
        if (data.status === "success") {
            loginPage.style.display = "none";
            dashboard.style.display = "block";
            const mid = document.getElementById("member-id");
            if (mid) mid.value = generateMemberID();
            loadMembers();
        } else {
            loginError.textContent = "Invalid username or password.";
        }
    })
    .catch(() => { loginError.textContent = "Server error. Is the server running?"; });
});

document.getElementById("sign-out-btn").addEventListener("click", () => {
    dashboard.style.display = "none";
    loginPage.style.display = "flex";
    loginForm.reset();
    loginError.textContent = "";
});

// ================= TABS =================
const tabBtns = document.querySelectorAll('.tab-btn');
const panels  = document.querySelectorAll('.tab-panel');
const titleEl = document.getElementById('panel-title');

const panelTitles = {
    'members-panel':      'Member Directory',
    'books-panel':        'Archive Inventory',
    'borrow-panel':       'Circulation Desk',
    'reservations-panel': 'Reservations Queue'
};

function switchTab(targetId) {
    tabBtns.forEach(b => b.classList.remove('active-nav'));
    const activeBtn = document.querySelector(`.tab-btn[data-target="${targetId}"]`);
    if (activeBtn) activeBtn.classList.add('active-nav');
    if (titleEl) titleEl.textContent = panelTitles[targetId] || 'Library CMS';
    panels.forEach(p => { p.classList.toggle('active', p.id === targetId); });
}

tabBtns.forEach(btn => btn.addEventListener('click', () => {
    const target = btn.getAttribute('data-target');
    switchTab(target);
    if (target === 'members-panel') loadMembers();
}));
switchTab('members-panel');

// ================= MEMBER MODAL =================
const memberModal    = document.getElementById("member-modal");
const openModalBtn   = document.getElementById("add-member-modal-btn");
const closeModalBtn  = document.getElementById("close-member-modal");
const cancelModalBtn = document.getElementById("cancel-member-modal");

function openMemberModal() {
    memberModal.style.display = 'flex';
    const mid = document.getElementById("member-id");
    if (mid && !mid.value) mid.value = generateMemberID();
}
function closeMemberModal() {
    memberModal.style.display = 'none';
}

if (openModalBtn)   openModalBtn.addEventListener('click', openMemberModal);
if (closeModalBtn)  closeModalBtn.addEventListener('click', closeMemberModal);
if (cancelModalBtn) cancelModalBtn.addEventListener('click', closeMemberModal);

// ================= MEMBERS =================
document.getElementById("add-member-btn").addEventListener("click", () => {
    const id      = document.getElementById("member-id").value.trim();
    const name    = document.getElementById("member-name").value.trim();
    const email   = document.getElementById("member-email").value.trim();
    const phone   = document.getElementById("member-phone").value.trim();
    const address = document.getElementById("member-address").value.trim();

    if (!id || !name) { alert("Member ID and Name are required."); return; }

    fetch('/api/members', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, name, email, phone, address })
    }).then(res => res.json()).then(() => {
        closeMemberModal();
        loadMembers();
        document.getElementById("member-id").value = generateMemberID();
    }).catch(() => alert("Error adding member."));
});

document.getElementById("display-members-btn").addEventListener("click", loadMembers);

function loadMembers() {
    fetch('/api/members')
        .then(res => res.json())
        .then(members => {
            const tbody = document.getElementById("members-table-body");
            if (!members.length) {
                tbody.innerHTML = `<tr><td colspan="5" class="px-6 py-12 text-center text-on-surface-variant italic text-body-sm">No members found.</td></tr>`;
                return;
            }
            members.reverse();
            tbody.innerHTML = members.map(m => `
                <tr class="hover:bg-surface-container-low transition-colors">
                    <td class="px-6 py-4 text-body-sm text-primary font-medium">${m.ID || m.id}</td>
                    <td class="px-6 py-4">
                        <div class="flex items-center gap-3">
                            <div class="w-8 h-8 rounded-full bg-secondary-container flex items-center justify-center font-bold text-[10px] text-on-secondary-container">${(m.Name||m.name||'?').split(' ').map(n=>n[0]).join('').slice(0,2).toUpperCase()}</div>
                            <span class="text-body-md text-on-surface">${m.Name || m.name}</span>
                        </div>
                    </td>
                    <td class="px-6 py-4 text-body-sm text-on-surface-variant">${m.Email || m.email || '—'}</td>
                    <td class="px-6 py-4 text-body-sm text-on-surface-variant">${m.Phone || m.phone || '—'}</td>
                    <td class="px-6 py-4 text-right">
                        <button onclick="removeMember('${m.ID || m.id}')" class="text-on-surface-variant hover:text-error p-1 transition-colors">
                            <span class="material-symbols-outlined text-[20px]">delete</span>
                        </button>
                    </td>
                </tr>`).join('');
        }).catch(() => alert("Error fetching members. Is the server running?"));
}

function removeMember(id) {
    if (!confirm(`Remove member ${id}?`)) return;
    fetch(`/api/members/${id}`, { method: 'DELETE' })
        .then(res => res.json())
        .then(data => {
            if (data.status === 'success') loadMembers();
            else alert("Member not found.");
        }).catch(() => alert("Error removing member."));
}

// ================= BOOKS =================
document.getElementById("add-book-btn").addEventListener("click", () => {
    const id     = Number(document.getElementById("book-id").value);
    const title  = document.getElementById("book-title").value.trim();
    const author = document.getElementById("book-author").value.trim();
    const genre  = document.getElementById("book-genre").value.trim();
    const copies = Number(document.getElementById("book-copies").value);

    if (!id || !title) { alert("Book ID and Title are required."); return; }

    fetch('/api/books', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id, title, author, genre, copies })
    }).then(res => res.json()).then(data => {
        if (data.status === 'success') loadBooks();
        else alert("Book ID already exists.");
    }).catch(() => alert("Error adding book."));
});

document.getElementById("remove-book-btn").addEventListener("click", () => {
    const id = Number(document.getElementById("book-id").value);
    if (!id) { alert("Enter a Book ID to remove."); return; }
    fetch(`/api/books/${id}`, { method: 'DELETE' })
        .then(res => res.json())
        .then(data => {
            if (data.status === 'success') loadBooks();
            else alert("Book not found.");
        }).catch(() => alert("Error removing book."));
});

document.getElementById("display-books-btn").addEventListener("click", loadBooks);

function loadBooks() {
    fetch('/api/books').then(res => res.json()).then(books => {
        const out = document.getElementById("books-output");
        if (!books.length) {
            out.innerHTML = `<div class="col-span-2 py-20 text-center text-on-surface-variant italic text-body-sm">No books in collection.</div>`;
            return;
        }
        books.reverse();
        out.innerHTML = books.map(b => `
            <div class="bg-white rounded-xl border border-outline-variant shadow-sm overflow-hidden group hover:shadow-md transition-shadow">
                <div class="p-5">
                    <div class="flex justify-between items-start mb-2">
                        <div>
                            <h5 class="text-title-md text-on-surface truncate pr-4">${b.Title || b.title}</h5>
                            <p class="text-body-sm text-outline">${b.Author || b.author}</p>
                        </div>
                        <span class="px-2 py-1 rounded text-[10px] font-bold uppercase ${(b.CopiesAvailable ?? b.copies) > 0 ? 'bg-secondary-container text-on-secondary-container' : 'bg-surface-container-high text-on-surface-variant'}">${(b.CopiesAvailable ?? b.copies) > 0 ? 'Available' : 'Borrowed'}</span>
                    </div>
                    <div class="flex items-center gap-4 mt-4 pt-4 border-t border-outline-variant/30">
                        <div class="flex flex-col"><span class="text-label-sm text-outline">ID</span><span class="text-label-md">${b.BookID || b.id}</span></div>
                        <div class="flex flex-col"><span class="text-label-sm text-outline">Genre</span><span class="text-label-md">${b.Genre || b.genre || '—'}</span></div>
                        <div class="flex flex-col"><span class="text-label-sm text-outline">Copies</span><span class="text-label-md">${b.CopiesAvailable ?? b.copies}</span></div>
                    </div>
                </div>
            </div>`).join('');
    }).catch(() => alert("Error fetching books. Is the server running?"));
}

// ================= BORROW =================
document.getElementById("add-borrow-btn").addEventListener("click", () => {
    const memberId = document.getElementById("borrow-member-id").value.trim();
    const bookId   = Number(document.getElementById("borrow-book-id").value);
    if (!memberId || !bookId) { alert("Enter Member ID and Book ID."); return; }
    fetch('/api/borrow', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ memberId, bookId })
    }).then(res => res.json()).then(data => {
        if (data.status === 'success') alert(`Book ${bookId} issued to ${memberId}.`);
        else alert("Borrow failed. Check IDs and availability.");
    }).catch(() => alert("Error issuing book."));
});

document.getElementById("view-history-btn").addEventListener("click", () => {
    const memberId = document.getElementById("borrow-member-id").value.trim();
    if (!memberId) { alert("Enter a Member ID to load history."); return; }
    fetch(`/api/borrow/${memberId}`)
        .then(res => res.json())
        .then(history => {
            const tbody = document.getElementById("borrow-table-body");
            if (!history.length) {
                tbody.innerHTML = `<tr><td colspan="4" class="px-6 py-12 text-center text-on-surface-variant italic text-body-sm">No history for member ${memberId}.</td></tr>`;
                return;
            }
            history.reverse();
            tbody.innerHTML = history.map(h => `
                <tr>
                    <td class="px-6 py-4 text-body-sm text-on-surface font-medium">${h.MemberID || h.memberId}</td>
                    <td class="px-6 py-4 text-body-sm text-on-surface">${h.BookID || h.bookId}</td>
                    <td class="px-6 py-4 text-body-sm text-on-surface-variant">${h.BorrowDate || h.borrowDate || '—'}</td>
                    <td class="px-6 py-4">
                        <span class="px-2 py-1 rounded-full text-[10px] uppercase font-bold ${(h.ReturnDate || h.returnDate) ? 'bg-secondary-container text-on-secondary-container' : 'bg-error-container text-on-error-container'}">${(h.ReturnDate || h.returnDate) ? 'Returned' : 'Active'}</span>
                    </td>
                </tr>`).join('');
        }).catch(() => alert("Error loading history. Is the server running?"));
});

// ================= RETURN =================
document.getElementById("return-book-btn").addEventListener("click", () => {
    const memberId = document.getElementById("return-member-id").value.trim();
    const bookId   = Number(document.getElementById("return-book-id").value);
    if (!memberId || !bookId) { alert("Enter Member ID and Book ID for return."); return; }
    fetch('/api/return', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ memberId, bookId })
    }).then(res => res.json()).then(() => alert("Return logged."))
    .catch(() => alert("Error logging return."));
});

document.getElementById("process-return-btn").addEventListener("click", () => {
    fetch('/api/process-return', { method: 'POST' })
        .then(res => res.json())
        .then(data => {
            if (data.status === 'empty') alert("No returns in the stack.");
            else alert(`Processed return: Book ${data.bookId} from Member ${data.memberId}.`);
        }).catch(() => alert("Error processing return."));
});

// ================= RESERVATIONS =================
document.getElementById("enqueue-reservation-btn").addEventListener("click", () => {
    const memberId = document.getElementById("reservation-member-id").value.trim();
    const bookId   = Number(document.getElementById("reservation-book-id").value);
    if (!memberId || !bookId) { alert("Enter Member ID and Book ID."); return; }
    fetch('/api/reserve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ memberId, bookId })
    }).then(res => res.json()).then(() => loadReservations())
    .catch(() => alert("Error adding reservation."));
});

document.getElementById("dequeue-reservation-btn").addEventListener("click", () => {
    fetch('/api/process-reservation', { method: 'POST' })
        .then(res => res.json())
        .then(data => {
            if (data.status === 'empty') alert("No reservations in queue.");
            else if (data.status === 'unavailable') alert(`Book ${data.bookId} unavailable for Member ${data.memberId}.`);
            else { alert(`Book ${data.bookId} assigned to Member ${data.memberId}.`); loadReservations(); }
        }).catch(() => alert("Error processing reservation."));
});

function loadReservations() {
    fetch('/api/reservations').then(res => res.json()).then(reservations => {
        const tbody = document.getElementById("reservations-table-body");
        if (!reservations.length) {
            tbody.innerHTML = `<tr><td colspan="3" class="px-6 py-12 text-center text-on-surface-variant italic text-body-sm">Queue is empty.</td></tr>`;
            return;
        }
        reservations.reverse();
        tbody.innerHTML = reservations.map((r, i) => `
            <tr>
                <td class="px-6 py-4">
                    <span class="flex items-center justify-center w-8 h-8 rounded-full ${i === 0 ? 'bg-primary-container text-on-primary-container' : 'bg-surface-container-high text-on-surface-variant'} text-label-md font-bold">${i + 1}</span>
                </td>
                <td class="px-6 py-4 text-body-md text-on-surface font-semibold">${r.MemberID || r.memberId}</td>
                <td class="px-6 py-4 text-body-md text-on-surface">${r.BookID || r.bookId}</td>
            </tr>`).join('');
    }).catch(() => {});
}
