import React from 'react';

export function Button({ 
  children, 
  onClick, 
  className = '', 
  variant = 'default' 
}) {
  const baseStyles = 'px-4 py-2 rounded-md font-medium transition-colors flex items-center';
  const variantStyles = {
    default: 'bg-blue-600 text-white hover:bg-blue-700',
    outline: 'border border-gray-300 dark:border-gray-600 hover:bg-gray-100 dark:hover:bg-gray-700'
  };

  return (
    <button
      onClick={onClick}
      className={`${baseStyles} ${variantStyles[variant]} ${className}`}
    >
      {children}
    </button>
  );
}
