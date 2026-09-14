/** @format */

$(function () {
	const MAX_STATIONS = 10;
	const SUGGESTION_DELAY = 500;

	const autocompleteConfig = {
		station: {
			placeholder: "Station Name",
			api: "/api/stations",
			data: [
				{ id: "station-1", name: "Alexanderplatz" },
				{ id: "station-2", name: "Berlin Hauptbahnhof" },
				{ id: "station-3", name: "Friedrichstraße" },
				{ id: "station-4", name: "Ostbahnhof" },
				{ id: "station-5", name: "Südkreuz" },
			],
		},

		line: {
			placeholder: "Line",
			api: "/api/lines",
			data: [
				{ id: "line-1", name: "U1" },
				{ id: "line-2", name: "U2" },
				{ id: "line-3", name: "U5" },
				{ id: "line-4", name: "U6" },
				{ id: "line-5", name: "S1" },
				{ id: "line-6", name: "S2" },
			],
		},

		direction: {
			placeholder: "Direction",
			api: "/api/directions",
			data: [
				{ id: "direction-1", name: "Pankow" },
				{ id: "direction-2", name: "Alexanderplatz" },
				{ id: "direction-3", name: "Hauptbahnhof" },
				{ id: "direction-4", name: "Rathaus Steglitz" },
			],
		},
	};

	const dependencies = {
		station: null,
		line: "station",
		direction: "line",
	};

	const dependents = {
		station: ["line", "direction"],
		line: ["direction"],
	};

	const $track = $(".carousel-track");
	const $items = $(".carousel-items");
	const $previous = $("#previous");
	const $next = $("#next");
	const $stationCount = $("#station-count");

	const validationState = {};
	const suggestionTimers = {};
	const suggestionRequests = {};

	let currentIndex = 0;

	// =========================================================
	// HELPERS
	// =========================================================

	function getField($input) {
		return $input.closest(".autocomplete-field");
	}

	function getType($field) {
		return $field.data("autocomplete-type");
	}

	function getKey($input) {
		const $field = getField($input);

		return `${$field.data("index")}-${getType($field)}`;
	}

	function getForm($input) {
		return $input.closest(".station-form");
	}

	function getInput($form, type) {
		return $form.find(
			`.autocomplete-field[data-autocomplete-type="${type}"] input[type="text"]`,
		);
	}

	function getIdInput($form, type) {
		return $form.find(
			`.autocomplete-field[data-autocomplete-type="${type}"] input[type="hidden"]`,
		);
	}
	function getSelectedId($input) {
		const $form = getForm($input);
		const type = getType(getField($input));
		return getIdInput($form, type).val();
	}
	function clearSelectedId($input) {
		const $form = getForm($input);
		const type = getType(getField($input));
		getIdInput($form, type).val("");
	}
	function setSelectedId($input, id) {
		const $form = getForm($input);
		const type = getType(getField($input));
		getIdInput($form, type).val(id);
	}

	function isValid($input) {
		return validationState[getKey($input)]?.valid === true;
	}

	function setValidity($input, valid) {
		const $field = getField($input);

		if (!$field.length) return;

		validationState[getKey($input)] = {
			valid,
			value: $.trim($input.val()),
		};

		$input.toggleClass("valid", valid).toggleClass("invalid", !valid);

		$field.toggleClass("valid", valid).toggleClass("invalid", !valid);

		$field
			.find(".input-container")
			.toggleClass("valid", valid)
			.toggleClass("invalid", !valid);
	}

	function clearValidity($input) {
		const $field = getField($input);

		if (!$field.length) return;

		delete validationState[getKey($input)];

		$input.removeClass("valid invalid");
		$field.removeClass("valid invalid");
		$field.find(".input-container").removeClass("valid invalid");
	}

	function dependenciesValid($input) {
		const dependency = dependencies[getType(getField($input))];

		return !dependency || isValid(getInput(getForm($input), dependency));
	}

	function invalidateField($form, type) {
		const $input = getInput($form, type);

		if (!$input.length) return;

		clearValidity($input);

		$input.val("").prop("disabled", true);

		const $field = getField($input);

		renderSuggestions($field, []);
		closeSuggestions($field);
	}

	function invalidateDependents($input) {
		const $form = getForm($input);
		const type = getType(getField($input));

		(dependents[type] || []).forEach((dependent) => {
			invalidateField($form, dependent);
		});
	}

	function updateDependencies($form) {
		const stationValid = isValid(getInput($form, "station"));
		const lineValid = isValid(getInput($form, "line"));

		getInput($form, "line").prop("disabled", !stationValid);
		getInput($form, "direction").prop("disabled", !lineValid);
	}

	// =========================================================
	// FORM ERRORS
	// =========================================================

	function setFormError($form, message) {
		$form.find(".error-message").text(message).toggle(!!message);
	}

	function clearFormError($form) {
		setFormError($form, "");
	}

	// =========================================================
	// CAROUSEL
	// =========================================================

	function generateCarousel() {
		$track.empty();
		$items.empty();

		for (let i = 0; i < MAX_STATIONS; i++) {
			$track.append(
				$("<span>", {
					class: "carousel-item",
					"data-index": i,
				}),
			);

			generateStationForm(i);
		}

		updateCarousel();
	}

	function generateStationForm(index) {
		const $form = $("<form>", {
			method: "get",
			action: "/edit-line",
			class: "station-form",
			id: `station-${index}`,
			"data-index": index,
		});

		$form.append(
			$("<h4>", {
				text: `Station ${index + 1}`,
			}),
		);

		$form.append(
			$("<input>", {
				type: "hidden",
				name: "stationIndex",
				value: index,
			}),
		);

		["station", "line", "direction"].forEach((type) => {
			$form.append(createAutocompleteField(type, index));
		});

		$form.append(
			$("<button>", {
				type: "submit",
				text: "Save Station",
			}),
		);

		$form.append(
			$("<p>", {
				class: "error-message",
			}),
		);

		$items.append($form);

		updateDependencies($form);
	}

	function updateCarousel() {
		const $forms = $(".station-form");
		const $carouselItems = $(".carousel-item");
		const total = $forms.length;

		if (!total) {
			$previous.addClass("deactive").prop("disabled", true);

			$next.addClass("deactive").prop("disabled", true);

			$stationCount.text("0/0");

			return;
		}

		currentIndex = Math.max(0, Math.min(currentIndex, total - 1));

		$forms.removeClass("active").eq(currentIndex).addClass("active");

		$carouselItems.removeClass("active").eq(currentIndex).addClass("active");

		$stationCount.text(`${currentIndex + 1}/${total}`);

		const atStart = currentIndex === 0;
		const atEnd = currentIndex === total - 1;

		$previous.toggleClass("deactive", atStart).prop("disabled", atStart);

		$next.toggleClass("deactive", atEnd).prop("disabled", atEnd);
	}

	$previous.on("click", function () {
		if (currentIndex > 0) {
			currentIndex--;
			updateCarousel();
		}
	});

	$next.on("click", function () {
		if (currentIndex < $(".station-form").length - 1) {
			currentIndex++;
			updateCarousel();
		}
	});

	$track.on("click", ".carousel-item", function () {
		const index = Number($(this).data("index"));
		const total = $(".station-form").length;

		if (Number.isInteger(index) && index >= 0 && index < total) {
			currentIndex = index;
			updateCarousel();
		}
	});

	// =========================================================
	// AUTOCOMPLETE
	// =========================================================

	function createAutocompleteField(type, index) {
		const config = autocompleteConfig[type];

		const $input = $("<input>", {
			type: "text",
			name: type,
			placeholder: config.placeholder,
			autocomplete: "off",
			required: true,
		});

		const $idInput = $("<input>", {
			type: "hidden",
			name: `${type}Id`,
		});

		const $field = $("<div>", {
			class: `${type}-field autocomplete-field`,
			"data-autocomplete-type": type,
			"data-index": index,
		});

		$field.append(
			$("<div>", {
				class: "input-container",
			}).append(
				$input,
				$idInput,
				$("<span>", {
					class: "checkmark",
				}),
			),
			$("<div>", {
				class: "suggestions",
			}),
		);

		return $field;
	}

	function renderSuggestions($field, suggestions) {
		$field.find(".suggestions").empty();
		suggestions.forEach((suggestion) => {
			const value = typeof suggestion === "object" ? suggestion.name : suggestion;
			const id = typeof suggestion === "object" ? suggestion.id : null;
			const $suggestion = $("<div>", { class: "suggestion", text: value });
			if (id !== null && id !== undefined) {
				$suggestion.attr("data-id", id);
			}
			$field.find(".suggestions").append($suggestion);
		});
	}

	function openSuggestions($field) {
		$field.addClass("active");
	}

	function closeSuggestions($field) {
		$field.removeClass("active").data("highlightedIndex", -1);

		updateHighlight($field);
	}

	function getVisibleSuggestions($field) {
		return $field.find(".suggestion:visible");
	}

	function filterSuggestions($input) {
		const $field = getField($input);

		if (!$field.length) return;

		const query = $.trim($input.val()).toLowerCase();
		let count = 0;

		$field.find(".suggestion").each(function () {
			const matches = $(this).text().trim().toLowerCase().includes(query);

			$(this).toggle(matches);

			if (matches) count++;
		});

		$field.data("highlightedIndex", -1);

		updateHighlight($field);

		count ? openSuggestions($field) : closeSuggestions($field);
	}

	function updateHighlight($field) {
		const $visible = getVisibleSuggestions($field);
		const index = $field.data("highlightedIndex") ?? -1;

		$field.find(".suggestion").removeClass("highlighted");

		if (index < 0 || index >= $visible.length) {
			return;
		}

		const $selected = $visible.eq(index);

		$selected.addClass("highlighted");

		$selected[0]?.scrollIntoView({
			block: "nearest",
		});
	}

	async function fetchSuggestions(type, query) {
		const config = autocompleteConfig[type];

		const normalizedQuery = $.trim(query).toLowerCase();

		return config.data.filter((item) =>
			item.name.toLowerCase().includes(normalizedQuery),
		);
	}

	async function updateSuggestions($input) {
		const $field = getField($input);

		if (!$field.length || $input.prop("disabled")) {
			return;
		}

		if (!dependenciesValid($input)) {
			renderSuggestions($field, []);
			closeSuggestions($field);
			return;
		}

		const query = $.trim($input.val());
		const key = getKey($input);
		const type = getType($field);

		const requestId = (suggestionRequests[key] || 0) + 1;
		suggestionRequests[key] = requestId;

		const suggestions = await fetchSuggestions(type, query);

		if (suggestionRequests[key] !== requestId || $.trim($input.val()) !== query) {
			return;
		}

		renderSuggestions($field, suggestions);

		if (query) {
			filterSuggestions($input);
		} else {
			suggestions.length ? openSuggestions($field) : closeSuggestions($field);
		}
	}

	function selectSuggestion($suggestion) {
		const $field = $suggestion.closest(".autocomplete-field");
		const $input = $field.find("input[type='text']");
		if (!dependenciesValid($input)) {
			return;
		}
		const id = $suggestion.attr("data-id");
		const value = $.trim($suggestion.text());
		if (!id) {
			return;
		}
		const key = getKey($input);
		clearTimeout(suggestionTimers[key]);
		delete suggestionTimers[key];
		suggestionRequests[key] = (suggestionRequests[key] || 0) + 1;
		$input.val(value).data("suggestion-selected", true);
		setSelectedId($input, id);
		setValidity($input, true);
		invalidateDependents($input);
		updateDependencies(getForm($input));
		renderSuggestions($field, []);
		closeSuggestions($field);
		$input.trigger("focus");
	}

	// =========================================================
	// EVENTS
	// =========================================================

	$(document).on("input", ".autocomplete-field input[type='text']", function () {
		const $input = $(this);
		const $field = getField($input);
		const key = getKey($input);

		clearValidity($input);
		clearSelectedId($input);

		invalidateDependents($input);
		updateDependencies(getForm($input));

		clearTimeout(suggestionTimers[key]);

		suggestionRequests[key] = (suggestionRequests[key] || 0) + 1;

		renderSuggestions($field, []);
		closeSuggestions($field);

		suggestionTimers[key] = setTimeout(() => {
			delete suggestionTimers[key];
			updateSuggestions($input);
		}, SUGGESTION_DELAY);
	});

	$(document).on("focus", ".autocomplete-field input", function () {
		const $input = $(this);
		const key = getKey($input);

		if ($input.prop("disabled") || !dependenciesValid($input)) {
			return;
		}

		if ($input.data("suggestion-selected")) {
			$input.removeData("suggestion-selected");

			return;
		}

		clearTimeout(suggestionTimers[key]);

		suggestionTimers[key] = setTimeout(() => {
			delete suggestionTimers[key];
			updateSuggestions($input);
		}, SUGGESTION_DELAY);
	});

	$(document).on("keydown", ".autocomplete-field input", function (event) {
		const $input = $(this);
		const $field = getField($input);
		const $visible = getVisibleSuggestions($field);

		if (event.key === "Escape") {
			closeSuggestions($field);
			return;
		}

		if (!["ArrowDown", "ArrowUp", "Enter"].includes(event.key)) {
			return;
		}

		if (event.key === "Enter") {
			const index = $field.data("highlightedIndex") ?? -1;

			if (index >= 0 && index < $visible.length) {
				event.preventDefault();

				selectSuggestion($visible.eq(index));
			}

			return;
		}

		event.preventDefault();

		if (!$visible.length) {
			return;
		}

		let index = $field.data("highlightedIndex") ?? -1;

		index += event.key === "ArrowDown" ? 1 : -1;

		if (index >= $visible.length) {
			index = 0;
		}

		if (index < 0) {
			index = $visible.length - 1;
		}

		$field.data("highlightedIndex", index);

		updateHighlight($field);
	});

	$(document).on("mousedown", ".autocomplete-field .suggestion", function (event) {
		event.preventDefault();

		selectSuggestion($(this));
	});

	$(document).on("mousedown", function (event) {
		const $target = $(event.target);

		$(".autocomplete-field").each(function () {
			const $field = $(this);

			if (!$target.is($field) && !$field.has($target).length) {
				closeSuggestions($field);
			}
		});
	});

	// =========================================================
	// FORM VALIDATION
	// =========================================================

	function validateForm($form) {
		const $inputs = $form.find(".autocomplete-field input[type='text']");

		const $invalidInput = $inputs
			.filter(function () {
				return !isValid($(this)) || !getSelectedId($(this));
			})
			.first();

		if ($invalidInput.length) {
			setFormError($form, "Please select a valid option for all fields.");

			$invalidInput.trigger("focus");
			return false;
		}

		clearFormError($form);
		return true;
	}

	// =========================================================
	// FORM SUBMISSION
	// =========================================================

	$(document).on("submit", ".station-form", function (event) {
		event.preventDefault();

		const $form = $(this);

		clearFormError($form);

		if (!validateForm($form)) {
			return;
		}

		const url = $form.attr("action");
		const method = $form.attr("method") || "GET";

		const $button = $form.find('[type="submit"]');

		$button.prop("disabled", true);

		$.ajax({
			url,
			method: method.toUpperCase(),
			data: $form.serialize(),
			dataType: "json",

			success: function (response) {
				/*
				 * Successful response.
				 *
				 * Example:
				 * {
				 *     success: true
				 * }
				 */

				if (response.success) {
					clearFormError($form);

					console.log("Station saved:", response);

					/*
					 * Add whatever should happen after
					 * a successful submission here.
					 */
				} else {
					/*
					 * Backend returned a valid response,
					 * but the operation failed.
					 *
					 * Example:
					 * {
					 *     success: false,
					 *     message: "Station already exists."
					 * }
					 */

					setFormError($form, response.message || "Something went wrong.");
				}
			},

			error: function (xhr) {
				/*
				 * Server/network error.
				 *
				 * If the backend returns:
				 *
				 * {
				 *     "message": "Station could not be added."
				 * }
				 *
				 * that message will be displayed.
				 */

				const message =
					xhr.responseJSON?.message || "Something went wrong. Please try again.";

				setFormError($form, message);
			},

			complete: function () {
				$button.prop("disabled", false);
			},
		});
	});

	// =========================================================
	// INITIALISE
	// =========================================================

	generateCarousel();
});
