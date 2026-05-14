tailwind.config = {
    darkMode: "class",
    theme: {
        extend: {
            "colors": {
                "primary": "#3525cd",
                "on-primary": "#ffffff",
                "primary-container": "#4f46e5",
                "on-primary-container": "#dad7ff",
                "primary-fixed": "#e2dfff",
                "primary-fixed-dim": "#c3c0ff",
                "secondary": "#006a61",
                "on-secondary": "#ffffff",
                "secondary-container": "#86f2e4",
                "on-secondary-container": "#006f66",
                "tertiary": "#3130c0",
                "on-tertiary": "#ffffff",
                "tertiary-container": "#4b4dd8",
                "on-tertiary-container": "#d9d8ff",
                "tertiary-fixed": "#e1e0ff",
                "tertiary-fixed-dim": "#c0c1ff",
                "error": "#ba1a1a",
                "on-error": "#ffffff",
                "error-container": "#ffdad6",
                "on-error-container": "#93000a",
                "surface": "#f7f9fb",
                "on-surface": "#191c1e",
                "surface-variant": "#e0e3e5",
                "on-surface-variant": "#464555",
                "surface-container-lowest": "#ffffff",
                "surface-container-low": "#f2f4f6",
                "surface-container": "#eceef0",
                "surface-container-high": "#e6e8ea",
                "surface-container-highest": "#e0e3e5",
                "surface-bright": "#f7f9fb",
                "surface-dim": "#d8dadc",
                "surface-tint": "#4d44e3",
                "outline": "#777587",
                "outline-variant": "#c7c4d8",
                "background": "#f7f9fb",
                "on-background": "#191c1e",
                "inverse-surface": "#2d3133",
                "inverse-on-surface": "#eff1f3",
                "inverse-primary": "#c3c0ff"
            },
            "spacing": {
                "sidebar_width": "260px",
                "stack_sm": "0.5rem",
                "stack_md": "1rem",
                "stack_lg": "2rem",
                "gutter": "1.5rem",
                "container_padding": "2rem"
            },
            "fontSize": {
                "display-lg": ["36px", {"lineHeight": "44px", "letterSpacing": "-0.02em", "fontWeight": "700"}],
                "headline-lg": ["24px", {"lineHeight": "32px", "letterSpacing": "-0.01em", "fontWeight": "600"}],
                "title-md": ["18px", {"lineHeight": "24px", "fontWeight": "600"}],
                "label-md": ["14px", {"lineHeight": "20px", "letterSpacing": "0.01em", "fontWeight": "500"}],
                "label-sm": ["12px", {"lineHeight": "16px", "fontWeight": "600"}],
                "body-md": ["16px", {"lineHeight": "24px", "fontWeight": "400"}],
                "body-sm": ["14px", {"lineHeight": "20px", "fontWeight": "400"}]
            }
        }
    }
};
