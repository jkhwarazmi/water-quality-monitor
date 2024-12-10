#pragma once

#include <QString>
#include <QApplication>
#include <QPalette>

namespace Styles {
  // Check if system is in dark mode
  inline bool isDarkMode() {
    return qApp->palette().color(QPalette::Window).lightness() < 128;
  }

  // Base colors
  inline QString backgroundColor() {
    return isDarkMode() ? "#1e272e" : "#ffffff";
  }

  inline QString textColor() {
    return isDarkMode() ? "#ffffff" : "#2c3e50";
  }

  inline QString secondaryTextColor() {
    return isDarkMode() ? "#bdc3c7" : "#6c757d";
  }

  // Interactive element colors
  inline QString primaryButtonBackground() {
    return isDarkMode() ? "#34495e" : "#e2e8f0";
  }

  inline QString primaryButtonBorder() {
    return isDarkMode() ? "#4a6480" : "#e9ecef";
  }

  inline QString primaryButtonHover() {
    return isDarkMode() ? "#4a6480" : "#e9ecef";
  }

  inline QString actionButtonBackground() {
    return isDarkMode() ? "#3498db" : "#007bff";
  }

  inline QString actionButtonHover() {
    return isDarkMode() ? "#2980b9" : "#0056b3";
  }

  inline QString actionButtonDisabled() {
    return isDarkMode() ? "#445566" : "#cccccc";
  }

  inline QString secondaryButtonBackground() {
    return isDarkMode() ? "#95a5a6" : "#6c757d";
  }

  inline QString secondaryButtonHover() {
    return isDarkMode() ? "#7f8c8d" : "#5a6268";
  }

  // Menu and tab colors
  inline QString menuBackground() {
    return isDarkMode() ? "#2c3e50" : "#f8f9fa";
  }

  inline QString menuHover() {
    return isDarkMode() ? "#34495e" : "#e9ecef";
  }

  inline QString tabBackground() {
    return isDarkMode() ? "#2c3e50" : "#e2e8f0";
  }

  inline QString selectedTabBackground() {
    return isDarkMode() ? "#3498db" : "#007bff";
  }

  // Table colors
  inline QString alternateRowBackground() {
    return isDarkMode() ? "#34495e" : "#f8f9fa";
  }

  inline QString tableHeaderBackground() {
    return isDarkMode() ? "#3498db" : "#007bff";
  }

  inline QString tableHeaderText() {
    return "#ffffff";  // White in both modes
  }

  inline QString selectionBackground() {
    return isDarkMode() ? "#3498db" : "#007bff";
  }

  inline QString selectionText() {
    return "#ffffff";  // White in both modes
  }

  // Border and grid colors
  inline QString borderColor() {
    return isDarkMode() ? "#4a6480" : "#dee2e6";
  }

  inline QString gridColor() {
    return isDarkMode() ? "#4a6480" : "#dee2e6";
  }

  // Constants
  const int BORDER_RADIUS = 6;        // Default border radius
  const int PADDING_SMALL = 8;        // Small padding (e.g., table cells)
  const int PADDING_MEDIUM = 10;      // Medium padding (e.g., buttons)
  const int PADDING_LARGE = 15;       // Large padding (e.g., containers)
  const int SPACING_SMALL = 5;        // Small spacing
  const int SPACING_MEDIUM = 10;      // Medium spacing
  const int SPACING_LARGE = 20;       // Large spacing
  const int BUTTON_MIN_WIDTH = 120;   // Minimum button width
  const int BUTTON_MIN_HEIGHT = 40;   // Minimum button height
  const int ACTION_BUTTON_WIDTH = 200; // Action button width
  const int ACTION_BUTTON_HEIGHT = 45; // Action button height
}