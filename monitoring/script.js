const firebaseConfig = {
    apiKey: "AIzaSyCk06v8QxT12eBR2Wkucyc0lC0uO-IOIpQ",
    databaseURL: "https://iotmonitoring-a99ff-default-rtdb.asia-southeast1.firebasedatabase.app"
};

firebase.initializeApp(firebaseConfig);
const database = firebase.database();

const ambangCahaya = 2000;
const ambangSuhu = 30.0;
const rooms = ['Ruangan1', 'Ruangan2'];

function buatKartuRuangan(roomId) {
    return `
        <div class="col-sm-12 col-md-6 col-lg-4 mb-4">
          <div class="card text-white" id="${roomId}-card" onclick="tampilkanDetail('${roomId}')">
            <div class="card-body">
              <h5 class="card-title">${roomId.replace('Ruangan', 'Ruangan ')}</h5>
              <p class="card-text" id="${roomId}-suhu">Suhu: Memuat...</p>
              <p class="card-text" id="${roomId}-gerakan">Aktivitas: Memuat...</p>
              <p class="card-text" id="${roomId}-cahaya">Cahaya: Memuat...</p>
              <p class="card-text" id="${roomId}-status">Status: Memeriksa...</p>
            </div>
          </div>
        </div>
      `;
}

function tampilkanDetail(roomId) {
    const suhu = document.getElementById(`${roomId}-suhu`).innerText;
    const gerakan = document.getElementById(`${roomId}-gerakan`).innerText;
    const cahaya = document.getElementById(`${roomId}-cahaya`).innerText;
    const status = document.getElementById(`${roomId}-status`).innerText;

    const modalBody = `
        <p><strong>Nama:</strong> ${roomId.replace('Ruangan', 'Ruangan ')}</p>
        <p><strong>${suhu}</strong></p>
        <p><strong>${gerakan}</strong></p>
        <p><strong>${cahaya}</strong></p>
        <p><strong>${status}</strong></p>
      `;

    document.getElementById('modalBody').innerHTML = modalBody;
    const modal = new bootstrap.Modal(document.getElementById('detailModal'));
    modal.show();
}

function cekRuangan(roomId) {
    const roomRef = database.ref(roomId);
    roomRef.once('value')
        .then((snapshot) => {
            const data = snapshot.val();
            if (!data) return;

            const suhu = data.suhu || 0;
            const gerakan = data.gerakan || 0;
            const cahaya = data.cahaya || 0;

            const aktivitas = gerakan === 1 ? "Ada aktivitas" : "Tidak ada aktivitas";

            document.getElementById(`${roomId}-suhu`).innerText = `Suhu: ${suhu}°C`;
            document.getElementById(`${roomId}-gerakan`).innerText = `Aktivitas: ${aktivitas}`;
            document.getElementById(`${roomId}-cahaya`).innerText = `Cahaya: ${cahaya}`;

            const statusEl = document.getElementById(`${roomId}-status`);
            const isBahaya = (gerakan === 0) &&
                ((suhu < ambangSuhu && cahaya < ambangCahaya) ||
                    (suhu < ambangSuhu && cahaya > ambangCahaya) ||
                    (suhu > ambangSuhu && cahaya < ambangCahaya));

            if (isBahaya) {
                statusEl.innerText = '⚠️ Mencurigakan';
                statusEl.className = 'card-text danger';
            } else {
                statusEl.innerText = '✅ Aman';
                statusEl.className = 'card-text safe';
            }
        })
        .catch((error) => {
            console.error(`❌ Gagal membaca ${roomId}:`, error);
        });
}

window.onload = () => {
    const dashboard = document.getElementById('dashboard');
    rooms.forEach(room => {
        dashboard.innerHTML += buatKartuRuangan(room);
        cekRuangan(room);
        setInterval(() => cekRuangan(room), 15000);
    });
};
