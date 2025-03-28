let generations = [];
let generationsRef = [];
let animationInterval;
let currentGeneration = 0;
let isPaused = true;

document.getElementById("mode").addEventListener("change", function() {
    const customOptions = document.getElementById("customOptions");
    customOptions.style.display = this.value === "custom" ? "flex" : "none";
    if (this.value === "auto") {
        document.getElementById("generations").value = "";
        document.getElementById("speed").value = "0.5";
    }
});

function createGrid(container, rows, cols) {
    
    container.innerHTML = '';
    container.style.gridTemplateRows = `repeat(${rows}, 20px)`;
    container.style.gridTemplateColumns = `repeat(${cols}, 20px)`;
    for (let i = 0; i < rows * cols; i++) {
        let cell = document.createElement("div");
        cell.classList.add("cell");
        container.appendChild(cell);
    }
}

function parseOutFile(content) {
    let separator = content.includes("\r\n\r\n") ? "\r\n\r\n" : "\n\n";
    let lineBreak = content.includes("\r\n") ? "\r\n" : "\n";
    return content.trim().split(separator).map(gen => gen.split(lineBreak).map(row => row.split("")));
}

document.getElementById("fileInput").addEventListener("change", function(event) {
    let file = event.target.files[0];
    if (file) {
        let reader = new FileReader();
        reader.onload = function(e) {
            generations = parseOutFile(e.target.result);
            if (generations.length > 0) {
                let rows = generations[0].length;
                let cols = generations[0][0].length;
                createGrid(document.getElementById("rightSide"), rows, cols);
                updateGrid(document.getElementById("rightSide"), generations[0]);
            }
        };
        reader.readAsText(file);
    }
});
document.getElementById("fileRef").addEventListener("change", function(event) {
    
    let file = event.target.files[0];
    if (file) {
        let reader = new FileReader();
        reader.onload = function(e) {
            generationsRef = parseOutFile(e.target.result);
            if (generationsRef.length > 0) {
                let rows = generationsRef[0].length;
                let cols = generationsRef[0][0].length;
                createGrid(document.getElementById("leftSide"), rows, cols);
                updateGrid(document.getElementById("leftSide"), generationsRef[0]);
            }
        };
        reader.readAsText(file);
    }
});

function updateGrid(container, generation) {
    let cells = container.children;
    let index = 0;
    for (let row of generation) {
        for (let cellChar of row) {
            cells[index].className = "cell"; 
            if (cellChar === 'X') {
                cells[index].classList.add("alive");
            } else if (cellChar === '+') {
                cells[index].classList.add("dead");
            }
            index++;
        }
    }
}
function max(var1,var2){
    return var1>var2?var1:var2;
}

function startAnimation() {
    let mode = document.getElementById("mode").value;
    let speed = mode === "custom" ? parseFloat(document.getElementById("speed").value) * 1000 : 500;
    let maxGen = mode === "custom" ? max(max(parseInt(document.getElementById("generations").value) , generations.length) , generationsRef.length) : max(generations.length , generationsRef.length);

    if (animationInterval) clearInterval(animationInterval);
    
    animationInterval = setInterval(() => {
        if (currentGeneration >= maxGen) {
            clearInterval(animationInterval);
        } else {
            document.getElementById("numGen").textContent = currentGeneration;
            if(currentGeneration < generationsRef.length) 
                updateGrid(document.getElementById("leftSide"), generationsRef[currentGeneration]);
            if(currentGeneration < generations.length)
                updateGrid(document.getElementById("rightSide"), generations[currentGeneration]);
            currentGeneration++;
            
        }
    }, speed);
}

document.getElementById("start").addEventListener("click", () => {
    currentGeneration = 0;
    isPaused = false;
    startAnimation();
} );
document.getElementById("pause").addEventListener("click", () => {
    if (isPaused) {
        startAnimation();
        document.getElementById("pause").textContent = "Pauza";
    } else {
        clearInterval(animationInterval);
        document.getElementById("pause").textContent = "Resume";
    }
    isPaused = !isPaused;
});
document.getElementById("reset").addEventListener("click", () => location.reload());