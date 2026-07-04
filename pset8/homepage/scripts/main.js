// Interactive behaviour for the CS50x pset8 Homepage.
//
// 1. A live clock in the footer, updated once per second.
// 2. A client-side-graded quiz on quiz.html (no server round-trip).

// --- Live footer clock ---------------------------------------------------------------
function updateClock() {
    const el = document.getElementById("clock");
    if (el) {
        el.textContent = new Date().toLocaleTimeString();
    }
}

// --- Quiz grading --------------------------------------------------------------------
function gradeQuiz(event) {
    event.preventDefault();

    const form = event.currentTarget;
    const fieldsets = form.querySelectorAll("fieldset[data-answer]");
    let correct = 0;

    fieldsets.forEach((fs) => {
        const expected = fs.dataset.answer;
        const chosen = fs.querySelector("input:checked");
        const legend = fs.querySelector("legend");

        legend.classList.remove("answer-correct", "answer-wrong");
        if (chosen && chosen.value === expected) {
            correct += 1;
            legend.classList.add("answer-correct");
        } else {
            legend.classList.add("answer-wrong");
        }
    });

    const total = fieldsets.length;
    const result = document.getElementById("result");
    result.classList.remove("d-none", "alert-success", "alert-warning", "alert-danger");
    result.classList.add(
        correct === total ? "alert-success" : correct === 0 ? "alert-danger" : "alert-warning"
    );
    result.textContent = `You got ${correct} out of ${total} correct.`;
}

// --- Wire everything up on load ------------------------------------------------------
document.addEventListener("DOMContentLoaded", () => {
    updateClock();
    setInterval(updateClock, 1000);

    const quiz = document.getElementById("quiz");
    if (quiz) {
        quiz.addEventListener("submit", gradeQuiz);
    }
});
